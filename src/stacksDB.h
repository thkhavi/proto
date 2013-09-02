/*
 * =====================================================================================
 *
 *       Filename:  stacksDB.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13-08-23 08:49:13 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Ryan F. McCormick, Sandra K. Truong
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */

#ifndef  STACKSDB_H_INC
#define  STACKSDB_H_INC

#include <vector>
#include <mysql++.h>
#include "sample.h"

void getSamples(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password, 
	       std::vector<Sample> * pvSamples_samples);

#endif   /* ----- #ifndef STACKSDB_H_INC  ----- */
