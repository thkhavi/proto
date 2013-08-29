/*
 * =====================================================================================
 *
 *       Filename:  stacksToVCF.cpp
 *
 *    Description:  Interacts with a Stacks database to generate VCF output
 *
 *        Version:  1.0
 *        Created:  13-08-23 08:29:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */
#include	<stdlib.h>
#include	<iostream>
#include	<unistd.h>
#include	"stacksDB.h"
#include	"biopRefAllele.h"

int main ( int argc, char *argv[] )
{
	const char *database, *server, *user, *password;
	int opt;

	while ((opt = getopt(argc, argv, "d:s:u:p")) != -1) {
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
				if (optopt == 'p') {password = "";}
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


	//Generate a whitelist of tags from the database whose SNPs will be output to the VCF
	std::vector<int> vi_whitelist = getWhitelist(database, server, user, password);

	//This vector is for testing 
	//static const int arr [] = {55,111,179,384,385,1256,1260};
	//std::vector<int> vi_tags (arr, arr + sizeof(arr) / sizeof(arr[0]) );
	//End test vector
	
	//Get the relevant SNP information from the whitelisted tags
	std::vector<SNP> vSNP_snps = getAllelesAndCoordinates(database, server, user, password, vi_whitelist);
	
	//Identify the reference allele for each SNP by system calls to biopieces
	vSNP_snps = getRefAllele(vSNP_snps);

	//Output in VCF (Variant Call Format)
	//Output the VCF header
	//TODO: Use a date function to generate the date
	//TODO: Include additional information in output
	std::cout << "##fileformat=VCFv4.1" << std::endl <<
		"##fileDate=20132608" << std::endl <<
		"##source=StacksDB" << std::endl <<
		"##INFO=<ID=DB,Number=1,Type=String,Description=\"Database of origin\"" << std::endl <<
		"##FILTER=<ID=80prg,Description=\"Found in less than 80\% of progeny\"" << std::endl <<
		"##FORMAT=<ID=PH,Number=1,Description=\"Placeholder\"" << std::endl <<
		"#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO\tFORMAT" << std::endl;
	//Output each SNP
	int outputCounter = 0;
	int errCounter = 0;
	for (size_t i = 0; i < vSNP_snps.size(); ++i) {
		//Check the SNP Flag
		if (vSNP_snps[i].getQualFlag() == "FAIL") {
			std::cerr << "SNP at " << vSNP_snps[i].getChr() << " " <<
				vSNP_snps[i].getCoord() << " not being output." << std::endl;
			errCounter++;
			continue;
		}

		std::cout << vSNP_snps[i].getChr() << "\t" << vSNP_snps[i].getCoord() << "\t" <<
			vSNP_snps[i].getChr() << "_" << vSNP_snps[i].getCoord() << "_" << vSNP_snps[i].getTag() << "\t" <<
			vSNP_snps[i].getRefAllele() << "\t" << vSNP_snps[i].getAltAllele() << "\t" <<
		       "0" << "\t" << "PASS" << "\t" << "DB=Test" << "\t" << "PH" << std::endl;
		outputCounter++;	
	}
	std::cerr << "Excluded " << errCounter << " SNP" << std::endl << std::endl;
	std::cerr << "Output " << outputCounter << " SNP to VCF." << std::endl << std::endl;
	return EXIT_SUCCESS;
}
