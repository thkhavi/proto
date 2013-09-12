/*
 * =====================================================================================
 *
 *       Filename:  site.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-29 02:55:36 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Ryan F. McCormick, Sandra K. Truong
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <utility>
#include <string>
#include "tag.h"
#include "site.h"

//Constructors
Site::Site(Tag rev_tag, Tag fwd_tag)
{
	site.first = rev_tag;
	site.second = fwd_tag;
}
Tag Site::getRev()
{
	return site.first;
}
Tag Site::getFwd()
{
	return site.second;
}

