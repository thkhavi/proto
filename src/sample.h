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

class Sample {
	public:
		//Constructors
		Sample();
		//Destructors
		//~Sample();
	private:
		int avgDepth;
		int id;
		std::string name;
		std::string flag;
};

#endif   /* ----- #ifndef SAMPLE_H_INC  ----- */

