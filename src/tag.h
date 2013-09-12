/*
 * =====================================================================================
 *
 *       Filename:  tag.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-29 02:55:18 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Ryan F. McCormick, Sandra K. Truong
 *   Organization: Texas A&M University 
 *
 * =====================================================================================
 */

#ifndef  TAG_H_INC
#define  TAG_H_INC
#include  <mysql++.h>


class Tag {
	public:
		//Constructors
		Tag();
		//Destructors
		//~Tag();
		void printTag();
		void setID(int i_id);
		void setChr(mysqlpp::String Str_chr);
		void setCoordinate(int i_coordinate);
		void setStrand(mysqlpp::String Str_strand);
		void setAvgDepth(int i_avgdepth);
		void setFlag(std::string Str_flag);
		int getCoordinate();
		int getAvgDepth();
		std::string getStrand();
		std::string getChr();
	private:
		int coordinate;
		int avgDepth;
		int id;
		std::string strand;
		std::string Chr;
		std::string flag;
};

#endif   /* ----- #ifndef TAG_H_INC  ----- */
