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
#include "tag.h"

//Constructors
Tag::Tag()
{
	coordinate = avgDepth = id = 0;
	strand = "null";
	chr = "null";
}

void Tag::printTag()
{
	std::cerr << "Tag: << std::endl <<
			"\tID: " << id << std::endl <<
			"\tChr: " << Chr << std::endl <<
			"\tCoordinate: " << coordinate << std::endl <<
			"\tStrand: " << strand << std::endl <<
			"\tAvgDepth: " << avgDepth << std::endl <<
			"\tFlag: " << flag << std::endl;
}

void Tag::setChr(mysqlpp::String Str_chr)
{
	Chr = std::string(Str_chr.data(), Str_chr.length());
}

void Tag::setID(int i_id)
{
	id = i_id;
}

void Tag::setAvgDepth(int i_avgdepth)
{
	avgDepth = i_avgdepth;
}
