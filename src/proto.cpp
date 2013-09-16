/*
 * =====================================================================================
 *
 *       Filename:  proto.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-29 03:13:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ryan F. McCormick, Sandra K. Truong
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>
#include "sample.h"
#include "tag.h"
#include "site.h"
#include "stacksDB.h"

//TODO: This dataset object may deserve its own header file at some point
//
//

class Dataset{
	public:
		int getAvgDepth() { return avgDepth; }
		void setAvgDepth(int avg) { avgDepth = avg; }
		int getstdDev() { return stdDev; }
		void setStdDev(int std) { stdDev = std; }	
	private:
		int avgDepth;
		int stdDev;
};

int main ( int argc, char *argv[] )
{
	//Containers for the data being processed, and pointers for passing to functions.
 	std::vector<Tag> vTag_tags;
	std::vector<Tag> *pvTag_tags = &vTag_tags;
	std::vector<Site> vSites_sites;
	std::vector<Site> *pvSites_sites = &vSites_sites;
	std::vector<Sample> vSamples_samples;
	std::vector<Sample> *pvSamples_samples = &vSamples_samples;
	
	//Iterators
	int i, j, k;
	
	//Variables for the command line
	const char *database, *server, *user, *password;
	int opt;
	
	//TODO:Clean up command line argument parsing; don't return 3 messages under three different conditions. Fix dealing with password
	//
	//Parse the command line
	//Temporary fix for the password:
	password = "";

	while ((opt = getopt(argc, argv, "d:s:u:p:")) != -1) {
		switch (opt) {
			case 'd':
				database = optarg;
				break;
			case 's':
				server = optarg;
				break;
			case 'u':
				user = optarg;
				break;
			//TODO: Fix dealing with password
			case 'p':
				password = optarg;
				break;
			case '?':
				if (optopt == 'd' || optopt == 's' || optopt || 'u') {
					std::cerr << std::endl <<
						"Designation of target database, server, and user required: " << std::endl <<
						"\tstacksToVCF -d databaseName -s serverName -u userName" << std::endl << std::endl;
					exit(EXIT_FAILURE);
				}
				break;
		}
	}
	if (argc == 1) {		
		std::cerr << std::endl <<
			"No arguments received. Designation of target database, server, and user required: " << std::endl <<
			"\tstacksToVCF -d databaseName -s serverName -u userName" << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
	if (argc <= 5) {
		std::cerr << std::endl <<
			"Insufficient arguments received. Designation of target database, server, and user required: " << std::endl <<
			"\tstacksToVCF -d databaseName -s serverName -u userName" << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cerr << "Running main with the following: " << std::endl <<
	"\tDatabase: " << database << std::endl <<
	"\tServer: " << server << std::endl <<
	"\tUser: " << user << std::endl <<
	"\tPassword: " << password << std::endl;
	//Command line parsing finished


	std::cerr << "Hello from main in proto" << std::endl;
	//Populate the sample container with samples, and determine their average depth
	getSamples(database, server, user, password, pvSamples_samples);

	std::cerr << "finished getSamples from in proto" << std::endl;
	for (i=0; i<vSamples_samples.size(); i++) {
		vSamples_samples[i].printSample();
	}

	//Populate the tag container with tags, including depth information from the samples
	getTags(database, server, user, password, pvSamples_samples, pvTag_tags);

	std::cerr << "finished getTags from in proto" << std::endl;
	for (i=0; i< 5 ;i++){
		vTag_tags[i].printTag();
	}

	//Populate the site container with sites based on coordinately related tags with high sequencing depth
	getSites(database, server, user, password, pvTag_tags, pvSites_sites);

	std::cerr << "finished getSites from in proto" << std::endl;
	for (i=0; i< 5 ;i++){
		(vSites_sites[i].getRev()).printTag();
		(vSites_sites[i].getFwd()).printTag();
	}

	
	//Determine which sites a sample has present or absent
	querySamples(database, server, user, password, pvSamples_samples, pvSites_sites);


	return EXIT_SUCCESS;
}
