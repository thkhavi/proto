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
 *       Compiler:  gcc
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
#include "SNP.h"


std::vector<int> getWhitelist (const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password)
{
	mysqlpp::String greeting("Running getWhitelist");
	std::cerr << greeting << std::endl;

	// Connect to the sample database.
	mysqlpp::Connection conn(false);
	if (conn.connect(pcc_db, pcc_server, pcc_user, pcc_password)) {
		//Determine the number of individuals the tag needs to be in the pass a quality threshold
		int indThresh;
		mysqlpp::Query sizeQuery = conn.query("SELECT sample_id FROM samples");
		if (mysqlpp::StoreQueryResult sizeRes = sizeQuery.store()) {
			//TODO: Modify to detect whether a "cross" or a "population" and adjust accordingly
			//i.e. subtract parents or not.
			indThresh = sizeRes.num_rows() * 0.80;
			std::cerr << "Number of individuals for tag to be included in set to " << indThresh << std::endl;
		}
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
			/*
			for (size_t i = 0; i < vi_tags.size(); ++i) {
				std::cerr << vi_tags[i] << std::endl;
			}*/	
        	}
        	else {
			std::cerr << "Failed to get item list: " << query.error() << std::endl;
            		//return 1;
        	}

        return vi_tags;
    	}
    	else {
		std::cerr << "DB connection failed: " << conn.error() << std::endl;
        	//return 1;
    	}
	//return EXIT_SUCCESS;
}


std::vector<SNP> getAllelesAndCoordinates(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password, 
	       std::vector<int> vi_tags)
{
	std::cerr << "Running getAllelesAndCoordinates" << std::endl;
	//for(size_t i=0; i<vi_tags.size(); ++i) {
	//	std::cerr << vi_tags[i] << std::endl;
	//}
	//Initialize the vector of SNPs
	std::vector<SNP> vSNP_snps;
	// Connect to the sample database.
	mysqlpp::Connection conn(false);
	if (conn.connect(pcc_db, pcc_server, pcc_user, pcc_password)) {
		
		//For each tag:
		for(size_t k=0; k<vi_tags.size(); ++k) {	
			//std::cerr << "On tag " << vi_tags[k] << std::endl;
			// Retrieve alleles
			mysqlpp::Query query = conn.query("SELECT tag_id, rank_1, rank_2, col \
					FROM catalog_snps \
					WHERE tag_id=%0:tagID \
					LIMIT 5");
			query.parse();
			SNP snp;
			if (mysqlpp::StoreQueryResult res = query.store(vi_tags[k])) {
				//std::cerr << "Query successful; parsing" << std::endl;
				//For each row in the query result
				for (size_t i = 0; i < res.num_rows(); ++i) {
					snp.setTag(res[i]["tag_id"]);
					snp.setOffset(res[i]["col"]);
					snp.setTmpAllele1(res[i]["rank_1"]);
					snp.setTmpAllele2(res[i]["rank_2"]);
					//snp.printSNP();
					//Set up a query to get bp and strandedness
					//Would using a join be faster here?
					mysqlpp::Query subquery = conn.query("SELECT tag_id, chr, bp, strand \
							FROM catalog_tags \
							WHERE tag_id=%0:tagID \
							LIMIT 1");
					subquery.parse();
					if (mysqlpp::StoreQueryResult subres = subquery.store(vi_tags[k])) {
						//std::cerr << "Subquery successful; parsing" << std::endl;
						//For each row in the query result
						for (size_t j=0; j < subres.num_rows(); ++j) {
							snp.setChr(subres[j]["chr"]);
							snp.setTagCoordinates(subres[j]["bp"]);
							snp.setStrand(subres[j]["strand"]);
							//snp.printSNP();
						}
					} // End subquery
					vSNP_snps.push_back(snp);
				}
			} //End if query
        		else {
				std::cerr << "Failed to get item list: " << query.error() << std::endl;
        		}
		} //End for each tag
		return vSNP_snps;
	} //End if connection

    	else {
		std::cerr << "DB connection failed: " << conn.error() << std::endl;
    	}

	//return EXIT_SUCCESS;
}
