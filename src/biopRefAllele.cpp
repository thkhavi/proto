/*
 * =====================================================================================
 *
 *       Filename:  biopRefAllele.cpp
 *
 *    Description:  Interacts with biopieces to set the reference allele
 *
 *        Version:  1.0
 *        Created:  13-08-26 09:32:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>
#include "biopRefAllele.h"

std::vector<SNP> getRefAllele(std::vector<SNP> vSNP_snps)
{
	std::cerr << "Running getRefAllele; this may take a while" << std::endl;
	for (size_t i = 0; i < vSNP_snps.size(); ++i) {
		//First calculate the correct coordinate
		if (vSNP_snps[i].getStrand() == "+") {
			vSNP_snps[i].setCoordinates(vSNP_snps[i].getTagCoord() + 1 + vSNP_snps[i].getOffset());
		}
		else if (vSNP_snps[i].getStrand() == "-") {
			vSNP_snps[i].setCoordinates(vSNP_snps[i].getTagCoord() - vSNP_snps[i].getOffset());
			vSNP_snps[i].flipStrand();
		}
		else { 
			std::cerr << "Strand inconclusive" << std::endl;
			vSNP_snps[i].setQualFlag("FAIL");
		}
		
		//Convert chromosome string to char *
		std::string str_chr = vSNP_snps[i].getChr();
		//Allocate memory
		char * chromosome = new char[str_chr.size() + 1];
		std::copy(str_chr.begin(), str_chr.end(), chromosome);
		chromosome[str_chr.size()] = '\0';
		
		//Convert coordinate int to char *
		int tmpcoord = vSNP_snps[i].getCoord();
		std::ostringstream ostr;
		ostr << tmpcoord;
		std::string str_coordinates = ostr.str();
		//Allocate memory
		char * coordinates = new char[str_coordinates.size() + 1];
		std::copy(str_coordinates.begin(), str_coordinates.end(), coordinates);
		coordinates[str_coordinates.size()] = '\0';
		
		//Run biopieces to extract nucleotide at position:
		//TODO: This is a very slow step in the program;
		//Find a more efficient way to extract the reference allele
		char command[200];
		sprintf(command, "get_genome_seq -g Sbi1 -c %s -b %s -l 1 | awk '$1 == \"SEQ:\" {print $2}'", chromosome, coordinates);
		char output[100];
		FILE *p = popen(command, "r");
		if(p != NULL) {
			if(fgets(output, sizeof(output), p) != NULL) {
				//Trim the newline character from the output.
				std::string tmp(output);
				tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
				//Use the output to set the reference allele
				vSNP_snps[i].setRefAllele(tmp);
				vSNP_snps[i].updateAlleles();
			}
		}
		pclose(p);
		//Free memory
		delete[] coordinates;
		delete[] chromosome;
	}
	return vSNP_snps;

}

