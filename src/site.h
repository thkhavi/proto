/*
 * =====================================================================================
 *
 *       Filename:  site.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-29 02:55:31 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Ryan F. McCormick, Sandra K. Truong
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */


#ifndef  SITE_H_INC
#define  SITE_H_INC

class Site{
	public:
		//Constructurs
		Site(Tag rev_tag, Tag fwd_tag);
		//Destructors
		//~Site();
		Tag getRev();
		Tag getFwd();
	private:
		std::pair<Tag, Tag> site;
};

#endif   /* ----- #ifndef SITE_H_INC  ----- */
