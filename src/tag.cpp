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
