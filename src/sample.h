/*
 * =====================================================================================
 *
 *       Filename:  sample.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-29 01:17:32 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Ryan F. McCormick, Sandra K. Truong
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */

#ifndef  SAMPLE_H_INC
#define  SAMPLE_H_INC

#include  <mysql++.h>

class Sample {
	public:
		//Constructors
		Sample();
		//Destructors
		//~Sample();
		void printSample();
		void setName(mysqlpp::String Str_name);
		void setID(int i_id);
		void setAvgDepth(float f_avgdepth);
		int getID();
		float getAvgDepth();
		std::string getName();

	private:
		float avgDepth;
		int id;
		std::string name;
		std::string flag;
};

#endif   /* ----- #ifndef SAMPLE_H_INC  ----- */

