/*
 * =====================================================================================
 *
 *       Filename:  SNP.cpp
 *
 *    Description:  Contains the SNP class
 *
 *        Version:  1.0
 *        Created:  13-08-23 05:12:19 PM
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
#include <string>
#include <mysql++.h>
#include "SNP.h"

//Constructors
SNP::SNP()
{
	coordinate = tag_coordinate = tag = 0;
	ref_allele = alt_allele = tmp_allele1 = tmp_allele2 = "N";
	chr = "chromosome_n";
	strand = "null";
	qualFlag = "null";
}
//General functions
void SNP::printSNP()
{
	std::cerr << "SNP info:" << std::endl <<
		"\t" << "Tag ID: " << tag << std::endl <<
		"\t" << "Chromosome: " << chr << std::endl <<
		"\t" << "Coordinate: " << coordinate << std::endl <<
		"\t" << "Tag coordinate: " << tag_coordinate << std::endl <<
		"\t" << "Strand: " << strand << std::endl <<
		"\t" << "Offset: " << offset << std::endl <<
		"\t" << "Reference allele: " << ref_allele << std::endl <<
		"\t" << "Alt allele: " << alt_allele << std::endl <<
		"\t" << "Tmp allele1: " << tmp_allele1 << std::endl <<
		"\t" << "Tmp allele2: " << tmp_allele2 << std::endl <<
		"\t" << "Quality flag " << qualFlag << std::endl;
}

void SNP::flipStrand()
{
	if (strand == "+") { std::cerr << "+ strand; no need to flip";}
	if (strand == "-") {
		if (tmp_allele1 == "A") { tmp_allele1 = "T";}
		else if (tmp_allele1 == "T") { tmp_allele1 = "A";}
		else if (tmp_allele1 == "G") { tmp_allele1 = "C";}
		else if (tmp_allele1 == "C") { tmp_allele1 = "G";}

		if (tmp_allele2 == "A") { tmp_allele2 = "T";}
		else if (tmp_allele2 == "T") { tmp_allele2 = "A";}
		else if (tmp_allele2 == "G") { tmp_allele2 = "C";}
		else if (tmp_allele2 == "C") { tmp_allele2 = "G";}
		}
		strand = "+";
	}

void SNP::updateAlleles()
{
	if (ref_allele == tmp_allele1) {
		alt_allele = tmp_allele2;
		setQualFlag("PASS");
	}
	else if (ref_allele == tmp_allele2) {
		alt_allele = tmp_allele1;
		setQualFlag("PASS");
	}
	else {
		std::cerr << "Warning: Tri-allelic SNP at " <<
		       chr << " " << coordinate << std::endl;
		setQualFlag("FAIL");
		printSNP();
	}
}

//Tag functions
void SNP::setTag(int tagID)
{
	tag = tagID;
}

int SNP::getTag()
{
	return tag;
}

//Chromosome functions
void SNP::setChr(mysqlpp::String chromosome)
{
	chr = std::string(chromosome.data(), chromosome.length());
}

std::string SNP::getChr()
{
	return chr;
}

void SNP::setStrand(mysqlpp::String strandtype)
{
	strand = std::string(strandtype.data(), strandtype.length());
}

std::string SNP::getStrand()
{
	return strand;
}

//Coordinate functions
void SNP::printCoordinates()
{
	std::cerr << "SNP coordinate: " << coordinate << std::endl;
}

void SNP::setCoordinates(int coord)
{
	coordinate = coord;
}

int SNP::getCoord()
{
	return coordinate;
}

void SNP::setTagCoordinates(int coord)
{
	tag_coordinate = coord;
}

int SNP::getTagCoord()
{
	return tag_coordinate;
}

void SNP::printOffset()
{
	std::cerr << "SNP offest: " << offset << std::endl;
}

void SNP::setOffset(int col)
{
	offset = col;
}

int SNP::getOffset()
{
	return offset;
}

//Allele functions
void SNP::printAlleles()
{
	std::cerr << "Reference allele: " << ref_allele << std::endl <<
		"Alternate allele: " << alt_allele << std::endl <<
		"tmp allele 1: " << tmp_allele1 << std::endl <<
		"tmp allele 2: " << tmp_allele2 << std::endl;
}
void SNP::setRefAllele(mysqlpp::String allele)
{
	ref_allele = std::string(allele.data(), allele.length());
}

void SNP::setRefAllele(std::string allele)
{
	ref_allele = allele;
}

std::string SNP::getRefAllele()
{
	return ref_allele;
}

void SNP::setAltAllele(mysqlpp::String allele)
{
	alt_allele = std::string(allele.data(), allele.length());
}

std::string SNP::getAltAllele()
{
	return alt_allele;
}

void SNP::setTmpAllele1(mysqlpp::String allele)
{
	tmp_allele1 = std::string(allele.data(), allele.length());
}

void SNP::setTmpAllele2(mysqlpp::String allele)
{
	tmp_allele2 = std::string(allele.data(), allele.length());
}

//Quality flag members
std::string SNP::getQualFlag()
{
	return qualFlag;
}

void SNP::setQualFlag(std::string flag)
{
	qualFlag = flag;
}

