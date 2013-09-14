/*
 * =====================================================================================
 *
 *       Filename:  stacksDB.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-23 08:43:19 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *        Authors:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *       	    Sandra K. Truong (), thkhavi@tamu.edu
 *   Organization:  Biochemistry & Biophysics, Texas A&M University
 *
 * =====================================================================================
 */
 
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <mysql++.h>
#include "stacksDB.h"
#include <map>
#include <vector>
#include <string>

 
void getSamples(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password, 
	       std::vector<Sample> * pvSamples_samples)
{
	std::cerr << "Hello from getSamples" << std::endl;
	//Iterators
	int i, j;
	
	int depthSum;
	int depthAvg;

	// Connect to the sample database.
	mysqlpp::Connection conn(false);
	if (conn.connect(pcc_db, pcc_server, pcc_user, pcc_password)) {
		//Execute the query for the samples, and process the return
		mysqlpp::Query sampleQuery = conn.query("SELECT sample_id, file FROM samples");

		if (mysqlpp::StoreQueryResult sampleRes = sampleQuery.store()) {
			for (i = 0; i < sampleRes.num_rows(); i++) {
				Sample sample;
				sample.setName(sampleRes[i]["file"]);
				sample.setID(sampleRes[i]["sample_id"]);
				
				//Retrieve the depth for every tag contained within the sample and calculate average
				depthSum = 0;
				mysqlpp::Query depthQuery = conn.query("SELECT depth FROM tag_index \
						WHERE sample_id=%0:idval");
				depthQuery.parse();
				if (mysqlpp::StoreQueryResult depthRes = depthQuery.store(sampleRes[i]["sample_id"])) {
						for (j = 0; j < depthRes.num_rows(); j++) {
							depthSum = depthSum + depthRes[j]["depth"];
						}
						depthAvg = depthSum / (depthRes.num_rows());
						sample.setAvgDepth(depthAvg);
				}
				pvSamples_samples->push_back(sample);
			}
		}
	}
}


void getTags(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password, 
	       std::vector<Sample> * pvSamples_samples,
	       std::vector<Tag> * pvTag_tags)
{
	std::cerr << "Hello from getTags" << std::endl;
	//Temporary holder to convert a mysqlpp::string to int.
	int tmpTagID;
	//Iterators
	int i, j, k, sample_count=0;
	
	int depthSum = 0;
	int depthAvg = 0;

	int avgDepthAllSamples = 0;

	//Calculate average depth of all samples
	for (i = 0; i < (*pvSamples_samples).size(); i++) {
		depthSum = depthSum + (*pvSamples_samples)[i].getAvgDepth();
	}
	avgDepthAllSamples = depthSum / pvSamples_samples->size() ;
	std::cerr << "\tCalculated average depth across all samples: " << avgDepthAllSamples << std::endl;
	
	// Connect to the sample database.
	mysqlpp::Connection conn(false);
	if (conn.connect(pcc_db, pcc_server, pcc_user, pcc_password)) {
		//Execute the query for the samples, and process the return
		mysqlpp::Query tagQuery = conn.query("SELECT tag_id, chr, bp, strand FROM catalog_tags");

	if (mysqlpp::StoreQueryResult tagRes = tagQuery.store()) {
			for (i = 0; i < tagRes.num_rows(); i++) {

				Tag tag;

				tag.setID(tagRes[i]["tag_id"]);
				tag.setChr(tagRes[i]["chr"]);
				tag.setCoordinate(tagRes[i]["bp"]);
				tag.setStrand(tagRes[i]["strand"]);

							
				//Retrieve the depth for every tag for all samples that are greater than or
				//equal to the average sample depth and calculate tag average depth
				depthSum = 0;
				for (k = 0; k < pvSamples_samples->size(); k++) {
					mysqlpp::Query depthQuery = conn.query("SELECT depth FROM tag_index \
							WHERE catalog_id=%0:catid AND sample_id=%1:tagid");
					depthQuery.parse();
					//For an unknown reason, using tagRes[i]["tag_id"] doesnt work directly in this query.store().
					//Maybe the arguments have to be of the same type for query.store()?
					tmpTagID = tagRes[i]["tag_id"];
					if (mysqlpp::StoreQueryResult depthRes = depthQuery.store(tmpTagID, (*pvSamples_samples)[k].getID())) {
						//This should only return one row
						for (j = 0; j < depthRes.num_rows(); j++) {
							//We should do something less stringent here
							if ((*pvSamples_samples)[k].getAvgDepth() >= avgDepthAllSamples) {
								depthSum = depthSum + depthRes[j]["depth"];
								sample_count++;
							}
						}
					}
				}
				if ( sample_count == 0 ){
					tag.setAvgDepth(0);
					tag.setFlag("-");
				}

				if ( sample_count > 0 ){
					depthAvg = depthSum / sample_count;
					sample_count=0;
					tag.setAvgDepth(depthAvg);
				}
				
				pvTag_tags->push_back(tag);
			}
		}
	}
}

void getSites(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password, 
	       std::vector<Tag> * pvTag_tags,
	       std::vector<Site> * pvSite_sites)
{
	std::cerr << "Hello from getSites" << std::endl;
	//Iterators
	int i;

	int depthSum = 0;
	int avgDepthAllTags = 0;
	
	//Define maps with key(coordinate,chr) value(tag)
	std::map< std::pair< int, std::string> , Tag> map_minus;
	std::map< std::pair< int, std::string> , Tag> map_plus;

	
	//Calculate average depth of all tags and populate maps
	for (i = 0; i < (*pvTag_tags).size(); i++) {
		//define key(coordinate, chr)
		std::pair<int,std::string> tagkey ((*pvTag_tags)[i].getCoordinate(),(*pvTag_tags)[i].getChr());
		//populate "+" strand
		if ((*pvTag_tags)[i].getStrand()=="+"){
			map_plus.insert(std::pair<std::pair<int,std::string> ,Tag>(tagkey, (*pvTag_tags)[i]));
		}
		//populate "-" strand
		if ((*pvTag_tags)[i].getStrand()=="-"){
			map_minus.insert(std::pair<std::pair<int,std::string> ,Tag>(tagkey,(*pvTag_tags)[i]));
		}
		depthSum = depthSum + (*pvTag_tags)[i].getAvgDepth();

	}
	avgDepthAllTags = depthSum / pvTag_tags->size() ;

	std::cerr << "\tCalculated average depth across all tags: " << avgDepthAllTags << std::endl;
	
	//Define sites as positions with both forward and reverse tags that have average depth to tags
	//For all tags
	
	typedef std::map<std::pair<int, std::string>, Tag> map_type;

	for (i = 0; i < (*pvTag_tags).size(); i++) {
		//may need to be more lenient about depth of tags
		if ((*pvTag_tags)[i].getStrand() == "-" && (*pvTag_tags)[i].getAvgDepth() >= avgDepthAllTags){
			//find exact coordinate and chromosome pair
			map_type::iterator it = map_plus.find(make_pair(((*pvTag_tags)[i].getCoordinate()+6),(*pvTag_tags)[i].getChr()));
			if(it != map_plus.end()){
				//may need to be more lenient about depth of tags
				if( (it->second).getAvgDepth() >= avgDepthAllTags){
					Site site((*pvTag_tags)[i],(it->second));
					pvSite_sites->push_back(site);
				}
   			}
		}
	}
}

void querySamples(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password,
	       std::vector<Sample> * pvSamples_samples,
	       std::vector<Site> * pvSite_sites)
{
	//iterators
	int i, j;

	//temporary ID holders

	int i_tempRevID, i_tempFwdID;
	//storage of querySamples
	std::vector< std::string > vstr_Temp;
	std::vector< std::vector<std::string> > vvstr_SampleStates;

	//to convert int to string
	int i_coordinate;		// number to be converted to a string
	std::string str_coordinate;	// string which will contain the result


	
	
	vstr_Temp.push_back("sample_name");
	for (j = 0; j < (*pvSite_sites).size(); j++){
		i_coordinate =((*pvSite_sites)[j].getRev()).getCoordinate();

		//convert int i_coordinate to string str_coordinate
		std::ostringstream convert; convert << i_coordinate; str_coordinate = convert.str(); 

		vstr_Temp.push_back(str_coordinate);
	}
	vvstr_SampleStates.push_back(vstr_Temp);
	vstr_Temp.clear();

	mysqlpp::Connection conn(false);
	if (conn.connect(pcc_db, pcc_server, pcc_user, pcc_password)) {

	for (i = 0; i < (*pvSamples_samples).size(); i++){
		
		vstr_Temp.push_back((*pvSamples_samples)[i].getName());
				
		for (j=0; j < (*pvSite_sites).size(); j++){

	
			i_tempRevID = ((*pvSite_sites)[j].getRev()).getID();
			i_tempFwdID = ((*pvSite_sites)[j].getFwd()).getID();

			mysqlpp::Query depthQuery = conn.query("SELECT catalog_id FROM tag_index \
							WHERE sample_id = %0:sampleid \
							AND ( catalog_id = %1 OR catalog_id = %2 )");
					depthQuery.parse();
					if (mysqlpp::StoreQueryResult depthRes = 
							depthQuery.store(
								(*pvSamples_samples)[i].getID(),i_tempRevID,i_tempFwdID)){
						// Both reverse and forward present, presence
						if (depthRes.num_rows() == 2){
							vstr_Temp.push_back("1");
						}
						// Either reverse or forward present, ambigious
						else if (depthRes.num_rows() ==1){
							vstr_Temp.push_back("-");
						}
						// Neither reverse of forward present, absence
						else if (depthRes.num_rows() == 0){
							vstr_Temp.push_back("0");
						}
						// Something unexpected happened, "WAT".
						else {vstr_Temp.push_back("wat");}
						
					}
					
		}
		vvstr_SampleStates.push_back(vstr_Temp);
		vstr_Temp.clear();
	}
	}
	for (i = 0; i < vvstr_SampleStates.size(); i++ ){
        	for (j=0; j<vvstr_SampleStates[i].size(); j++){
            		if(j==(vvstr_SampleStates[i].size()-1)){
				std::cout << vvstr_SampleStates[i][j] << std::endl;
            		}
            		else{
				std::cout << vvstr_SampleStates[i][j] << "\t";
            		}
        	}
    	}	
}

