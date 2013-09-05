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
 *         Author:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *   Organization:  
 *
 * =====================================================================================
 */
 
#include <stdlib.h>
#include <iostream>
#include <mysql++.h>
#include "stacksDB.h"

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
	//Iterators
	int i, j, k, sample_count=0;
	
	int depthSum = 0;
	int depthAvg = 0;

	int avgDepthAllSamples = 0;
	
	//Calculate average depth of all samples
	for (i = 0; i < (*pvSamples_samples).size(); i++) {
		depthSum = depthSum + (*pvSamples_samples)[i].getAvgDepth();
	}
	avgDepthAllSamples = depthSum / (*pvSamples_samples).size() ;
	
	
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
				mysqlpp::Query depthQuery = conn.query("SELECT catalog_id, sample_id, depth FROM tag_index \
						WHERE catalog_id=%0:idval");
				depthQuery.parse();
				if (mysqlpp::StoreQueryResult depthRes = depthQuery.store(tagRes[i]["tag_id"])) {


					for (j = 0; j < depthRes.num_rows(); j++) {
					
						Sample sample;
						sample.setID(depthRes[j]["sample_id"]);

						for (k = 0; k < (*pvSamples_samples).size(); k++) {
							if (sample.getID() == (*pvSamples_samples)[k].getID()){
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
				}
				pvTag_tags->push_back(tag);
			}
		}
	}
}
