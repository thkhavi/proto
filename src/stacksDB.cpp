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
	
	float depthSum;
	float depthAvg;

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
						WHERE sample_id=%0:idval\
					       		AND (catalog_id = 1 OR catalog_id = 2\
							OR catalog_id = 3 OR catalog_id = 4 OR catalog_id = 5\
							OR catalog_id = 6 OR catalog_id = 7\
							OR catalog_id = 8 OR catalog_id = 9 OR catalog_id = 10\
							OR catalog_id = 11 OR catalog_id = 12\
							OR catalog_id = 13 OR catalog_id =14 OR catalog_id = 15)");
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
	int i, j, k, sample_count=0, count_sample_0=0;

	
	float depthSum = 0;
	float depthAvg = 0;

	float avgDepthAllSamples = 0;

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
		mysqlpp::Query tagQuery = conn.query("SELECT tag_id, chr, bp, strand FROM catalog_tags LIMIT 15");
	
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
							WHERE catalog_id=%0:catid AND sample_id=%1:sampleid");
					depthQuery.parse();
					//For an unknown reason, using tagRes[i]["tag_id"] doesnt work directly in this query.store().
					//Maybe the arguments have to be of the same type for query.store()?
					tmpTagID = tagRes[i]["tag_id"];
					if (mysqlpp::StoreQueryResult depthRes = depthQuery.store(tmpTagID, (*pvSamples_samples)[k].getID())) {
						//This should only return one row
						for (j = 0; j < depthRes.num_rows(); j++) {
							//We should do something less stringent here
							//if ((*pvSamples_samples)[k].getAvgDepth() >= avgDepthAllSamples) {
								depthSum = depthSum + depthRes[j]["depth"];
								sample_count++;
							//}
						}
					}
				}
				if ( sample_count == 0){
					count_sample_0++;
				}

				if ( sample_count > 0 ){
					depthAvg = depthSum / sample_count;
					sample_count=0;
					tag.setAvgDepth(depthAvg);
					pvTag_tags->push_back(tag);
				}
				
				
			}
		}
	}
	std::cerr <<"\t\t****Tags that have sample count == 0: "<<count_sample_0<<std::endl; 
}

void getSites(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password, 
	       std::vector<Tag> * pvTag_tags,
	       std::vector<Site> * pvSite_sites)
{
	std::cerr << "Hello from getSites" << std::endl;
	//Iterators
	int i;

	float depthSum = 0;
	float avgDepthAllTags = 0;
	
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

	std::cerr << "\tSum of all tag depth: " << depthSum << std::endl;
	std::cerr << "\t# of tags: " << pvTag_tags->size() << std::endl;

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

void querySamplesSites(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password,
	       std::vector<Sample> * pvSamples_samples,
	       std::vector<Site> * pvSite_sites)
{
	//iterators
	int i, j, k;
	
	//counters for states:
	int count_presence, count_ambiguous, count_absence, count_wat;
	int count_ambiguous_fwd;
	//avd depth of presence and ambiguous
	float avgDepthPresence, avgDepthAmbiguous;

	//temporary ID holders
	int i_coordinate, i_RevID, i_FwdID, i_catID;
	std::string str_chr, str_coordinate, str_RevID, str_FwdID;

	//storage of querySamples
	std::vector< std::string > vstr_Temp;
	std::vector< std::vector<std::string> > vvstr_SampleStates;
	
	vstr_Temp.push_back("sample_name");
	for (j = 0; j < (*pvSite_sites).size(); j++){
		
		i_coordinate =((*pvSite_sites)[j].getRev()).getCoordinate();
		std::ostringstream convert_coordinate;
		convert_coordinate << i_coordinate; str_coordinate = convert_coordinate.str();

		i_RevID = ((*pvSite_sites)[j].getRev()).getID();
		std::ostringstream convert_RevID;
		convert_RevID << i_RevID; str_RevID = convert_RevID.str();		

		i_FwdID = ((*pvSite_sites)[j].getFwd()).getID();
		std::ostringstream convert_FwdID;
		convert_FwdID << i_FwdID; str_FwdID = convert_FwdID.str();
		
		str_chr = ((*pvSite_sites)[j].getRev()).getChr();

		vstr_Temp.push_back(str_chr+"_"+str_coordinate + "_" + str_RevID +"_"+ str_FwdID);
	
	}

	vvstr_SampleStates.push_back(vstr_Temp);
	vstr_Temp.clear();

	mysqlpp::Connection conn(false);
	if (conn.connect(pcc_db, pcc_server, pcc_user, pcc_password)) {

	for (i = 0; i < (*pvSamples_samples).size(); i++){

		count_presence=count_absence=count_ambiguous=count_wat=0;
		avgDepthPresence=avgDepthAmbiguous=0;
		count_ambiguous_fwd=0;
		
		vstr_Temp.push_back((*pvSamples_samples)[i].getName());
				
		for (j=0; j < (*pvSite_sites).size(); j++){

	
			i_RevID = ((*pvSite_sites)[j].getRev()).getID();
			i_FwdID = ((*pvSite_sites)[j].getFwd()).getID();

			mysqlpp::Query depthQuery = conn.query("SELECT catalog_id, depth FROM tag_index \
							WHERE sample_id = %0:sampleid \
							AND ( catalog_id = %1 OR catalog_id = %2 )");
					depthQuery.parse();
					if (mysqlpp::StoreQueryResult depthRes = 
							depthQuery.store(
								(*pvSamples_samples)[i].getID(),i_RevID,i_FwdID)){
						// Both reverse and forward present, presence
						if (depthRes.num_rows() == 2){
							vstr_Temp.push_back("1");
							for (k = 0; k < depthRes.num_rows(); k++){
								avgDepthPresence = avgDepthPresence + depthRes[k]["depth"];
							}
							count_presence++;
						}
						// Either reverse or forward present, ambigious
						else if (depthRes.num_rows() ==1){
							vstr_Temp.push_back("1");
							for (k = 0; k < depthRes.num_rows(); k++){
								avgDepthAmbiguous = avgDepthAmbiguous + depthRes[k]["depth"];
								i_catID = depthRes[k]["catalog_id"];
								if (i_catID == i_FwdID){
									count_ambiguous_fwd++;
								}
							}
							count_ambiguous++;

						}
						// Neither reverse of forward present, absence
						else if (depthRes.num_rows() == 0){
							vstr_Temp.push_back("0");
							count_absence++;

						}
						// Something unexpected happened, "WAT".
						else {vstr_Temp.push_back("wat"); count_wat++;}
						
					}
					
		}
		vvstr_SampleStates.push_back(vstr_Temp);
		vstr_Temp.clear();
		if (count_ambiguous != 0){avgDepthAmbiguous = avgDepthAmbiguous/count_ambiguous;}
		avgDepthPresence = avgDepthPresence/(2*count_presence);
		std::cerr << "For sample id " << (*pvSamples_samples)[i].getName() << std::endl;
		std::cerr << "\t" << count_presence << " sites present" << std::endl;
		std::cerr << "\t\t" << avgDepthPresence << " depth of sites present" << std::endl;
		std::cerr << "\t" << count_absence << " sites absent" << std::endl;
		std::cerr << "\t" << count_ambiguous << " sites ambigious" << std::endl;
		std::cerr << "\t\t" << avgDepthAmbiguous << " depth of sites ambiguous" << std::endl;
		std::cerr << "\t\t" << count_ambiguous_fwd << " fwd tags seen in ambiguous sites" << std::endl;
		std::cerr << "\t" << count_wat <<" wats...uhoh, don't be sad!" << std::endl;
		

	}
	}

	for (i = 0; i < vvstr_SampleStates.size(); i++ ){
        	for (j=0; j<vvstr_SampleStates[i].size(); j++){
			std::cout << vvstr_SampleStates[i][j] << "\t";
        	}
		std::cout << std::endl;
    	}

}

void querySamplesTags(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password,
	       std::vector<Sample> * pvSamples_samples,
	       std::vector<Tag> * pvTag_tags)
{
	//iterators
	int i, j, k;
	
	//counters for states:
	int count_presence, count_absence, count_wat;

	//temporary ID holders
	int i_coordinate, i_tagID;
	std::string str_chr, str_coordinate;

	//storage of querySamples
	std::vector< std::string > vstr_Temp;
	std::vector< std::vector<std::string> > vvstr_SampleStates;
	// to calculate average Tag depth
	float depthSum = 0;
	float avgDepthAllTags = 0;
		
	//Calculate average depth of all tags
	for (i = 0; i < (*pvTag_tags).size(); i++) {
		depthSum = depthSum + (*pvTag_tags)[i].getAvgDepth();
	}
	avgDepthAllTags = depthSum / pvTag_tags->size();

	
	vstr_Temp.push_back("sample_name");
	for (j = 0; j < (*pvTag_tags).size(); j++){
		// Tags must pass a certain depth threshold to be considered
		if ( (*pvTag_tags)[j].getAvgDepth() >=0.5*avgDepthAllTags){
		
			i_coordinate =(*pvTag_tags)[j].getCoordinate();
			std::ostringstream convert_coordinate;
			convert_coordinate << i_coordinate; str_coordinate = convert_coordinate.str();

			str_chr =(*pvTag_tags)[j].getChr();

			vstr_Temp.push_back(str_chr+"_bp_"+str_coordinate);
		}
	
	}
	
	std::cerr << "# of tags passed depth threshold: " << vstr_Temp.size()-1 <<std::endl;
	vvstr_SampleStates.push_back(vstr_Temp);
	vstr_Temp.clear();

	mysqlpp::Connection conn(false);
	if (conn.connect(pcc_db, pcc_server, pcc_user, pcc_password)) {

	for (i = 0; i < (*pvSamples_samples).size(); i++){

		count_presence=count_absence=count_wat=0;
		
		vstr_Temp.push_back((*pvSamples_samples)[i].getName());
				
		for (j=0; j < (*pvTag_tags).size(); j++){
			// Tags must pass a certain depth threshold to be considered
			if ( (*pvTag_tags)[j].getAvgDepth() >=0.5*avgDepthAllTags){

				i_tagID = (*pvTag_tags)[j].getID();
				mysqlpp::Query depthQuery = conn.query("SELECT catalog_id, depth FROM tag_index \
							WHERE sample_id = %0:sampleid \
							AND catalog_id = %1");
				depthQuery.parse();
				if (mysqlpp::StoreQueryResult depthRes = 
						depthQuery.store((*pvSamples_samples)[i].getID(),i_tagID)){
					// If tag is present, presence
					if (depthRes.num_rows() == 1){
						vstr_Temp.push_back("1");
						count_presence++;
					}
					// If tag is absent, absence
					else if (depthRes.num_rows() == 0){
						vstr_Temp.push_back("0");
						count_absence++;
					}
					// Something unexpected happened, "WAT".
					else {vstr_Temp.push_back("wat"); count_wat++;}
						
				}		
			}
		}
		vvstr_SampleStates.push_back(vstr_Temp);
		vstr_Temp.clear();
		std::cerr << "For sample id " << (*pvSamples_samples)[i].getName() << std::endl;
		std::cerr << "\t" << count_presence << " tags present" << std::endl;
		std::cerr << "\t" << count_absence << " tags absent" << std::endl;
		std::cerr << "\t" << count_wat <<" wats...uhoh, don't be sad!" << std::endl;
	}
	}
	for (i = 0; i < vvstr_SampleStates.size(); i++ ){
        	for (j=0; j<vvstr_SampleStates[i].size(); j++){
				std::cout << vvstr_SampleStates[i][j] << "\t";
        	}
		std::cout << std::endl;
    	}
}


