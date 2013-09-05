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

class Tag {
	public:
		//Constructors
		Tag();
		//Destructors
		//~Tag();
	private:
		int coordinate;
		int avgDepth;
		int id;
		std::string strand;
		std::string chr;
		std::string flag;
};
#endif   /* ----- #ifndef TAG_H_INC  ----- */
