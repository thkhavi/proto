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
}

void Tag::printTag()
{
	std::cerr << "Tag: << std::endl <<
			"\tID: " << id << std::endl <<
			"\tAvgDepth: " << avgDepth << std::endl <<
			"\tName: " << name << std::endl <<
			"\tFlag: " << flag << std::endl;
}

void Tag::setName(mysqlpp::String Str_name)
{
	name = std::string(Str_name.data(), Str_name.length());
}

void Tag::setID(int i_id)
{
	id = i_id;
}

void Tag::setAvgDepth(int i_avgdepth)
{
	avgDepth = i_avgdepth;
}
