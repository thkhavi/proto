/*
 * =====================================================================================
 *
 *       Filename:  tag.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-29 02:55:23 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Ryan F. McCormick, Sandra K. Truong
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string>
#include <utility>
#include "tag.h"

//Constructors


Tag::Tag()
{
	avgDepth = 0.0;
	coordinate = id = 0;
	strand = Chr = flag = "null";
}

void Tag::printTag()
{
	std::cerr << "Tag: "<< std::endl <<
			"\tID: " << id << std::endl <<
			"\tChr: " << Chr << std::endl <<
			"\tCoordinate: " << coordinate << std::endl <<
			"\tStrand: " << strand << std::endl <<
			"\tAvgDepth: " << avgDepth << std::endl <<
			"\tFlag: " << flag << std::endl;
}

void Tag::setID(int i_id)
{
	id = i_id;
}

void Tag::setChr(mysqlpp::String Str_chr)
{
	Chr = std::string(Str_chr.data(), Str_chr.length());
}

void Tag::setCoordinate(int i_coordinate)
{
	coordinate = i_coordinate;
}

void Tag::setStrand(mysqlpp::String Str_strand)
{
	strand = std::string(Str_strand.data(), Str_strand.length());
}

void Tag::setAvgDepth(float f_avgdepth)
{
	avgDepth = f_avgdepth;
}

void Tag::setFlag(std::string Str_flag)
{
	flag = Str_flag;
}

int Tag::getID()
{
	return id;
}
float Tag::getAvgDepth()
{
	return avgDepth;
}

int Tag::getCoordinate()
{
	return coordinate;
}

std::string Tag::getStrand()
{
	return strand;
}

std::string Tag::getChr()
{
	return Chr;
}
