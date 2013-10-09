/*
 * =====================================================================================
 *
 *       Filename:  sample.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-29 02:55:07 PM
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
#include <iostream>
#include <mysql++.h>
#include "sample.h"

Sample::Sample()
{
	avgDepth = 0.0;
       	id = 0;
	name = flag = "null";
}

void Sample::printSample()
{
	std::cerr << "Sample: " << std::endl <<
		"\tID: " << id << std::endl <<
		"\tAvgDepth: " << avgDepth << std::endl <<
		"\tName: " << name << std::endl <<
		"\tFlag: " << flag << std::endl;
}

void Sample::setName(mysqlpp::String Str_name)
{
	name = std::string(Str_name.data(), Str_name.length());
}

void Sample::setID(int i_id)
{
	id = i_id;
}

void Sample::setAvgDepth(float f_avgdepth)
{
	avgDepth = f_avgdepth;
}

int Sample::getID()
{
	return id;
}
float Sample::getAvgDepth()
{
	return avgDepth;
}
std::string Sample::getName()
{
	return name;
}

