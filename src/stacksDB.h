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
 *       Compiler:  gcc
 *
 *         Author:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  STACKSDB_H_INC
#define  STACKSDB_H_INC

#include <vector>
#include <mysql++.h>
#include "SNP.h"

std::vector<int> getWhitelist (const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password);

std::vector<SNP> getAllelesAndCoordinates(const char *pcc_db, const char *pcc_server,
	       const char *pcc_user, const char *pcc_password, 
	       std::vector<int> vi_tags);


#endif   /* ----- #ifndef STACKSDB_H_INC  ----- */
