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
