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
//Old code for an example:
		/* 
		// Retrieve high quality tags
		mysqlpp::Query query = conn.query("SELECT tag_id \
				FROM catalog_index \
				WHERE geno_cnt>=%0:thresh AND parents=2 AND snps<=5 AND \
				marker=\'aa/bb\'");
		query.parse();
		//Store the tags
		std::vector<int> vi_tags;
		if (mysqlpp::StoreQueryResult res = query.store(indThresh)) {
			for (size_t i = 0; i < res.num_rows(); ++i) {
				vi_tags.push_back(res[i]["tag_id"]);
				//std::cerr << res[i]["tag_id"] << std::endl;
			}
			std::cerr << "Number of whitelisted tags returned " << res.num_rows() << std::endl;
        	}
        	else {
			std::cerr << "Failed to get item list: " << query.error() << std::endl;
            		//return 1;
        	}

        //return vi_tags;
    	}
    	else {
		std::cerr << "DB connection failed: " << conn.error() << std::endl;
        	//return 1; */
    //	}
	//return EXIT_SUCCESS;
//}
