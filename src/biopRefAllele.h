/*
 * =====================================================================================
 *
 *       Filename:  biopRefAllele.h
 *
 *    Description:  Interacts with biopieces to set the reference allele
 *
 *        Version:  1.0
 *        Created:  13-08-26 09:36:28 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *   Organization:  Texas A&M University
 *
 * =====================================================================================
 */

#ifndef  BIOPREFALLELE_H_INC
#define BIOPREFALLELE_H_INC

#include <vector>
#include <mysql++.h>
#include "SNP.h"

std::vector<SNP> getRefAllele(std::vector<SNP> vSNP_snps);

#endif   /* ----- #ifndef BIOPREFALLELE_H_INC ----- */
