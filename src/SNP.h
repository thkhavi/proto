/*
 * =====================================================================================
 *
 *       Filename:  SNP.h
 *
 *    Description:  Header file for the SNP class
 *
 *        Version:  1.0
 *        Created:  13-08-23 05:14:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ryan F. McCormick (), ryanabashbash@tamu.edu
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  SNP_H_INC
#define  SNP_H_INC

class SNP {
	public:
		//Constructors
		SNP();
		//Destructors
		//~SNP();
		//General members
		void printSNP();
		void flipStrand();
		void updateAlleles();
		//Members for tag
		void setTag(int tagID);
		int getTag();
		//Members for coordinates
		void setStrand(mysqlpp::String strandtype);
		std::string getStrand();
		void setChr(mysqlpp::String chromosome);
		std::string getChr();
		void printCoordinates();
		void setCoordinates(int coord);
		void setTagCoordinates(int coord);
		int getCoord();
		int getTagCoord();
		void printOffset();
		void setOffset(int col);
		int getOffset();
		//Members for alleles
		void printAlleles();
		void setRefAllele(mysqlpp::String allele);
		std::string getRefAllele();
	     	void setRefAllele(std::string allele);	
		void setAltAllele(mysqlpp::String allele);
		std::string getAltAllele();
		void setTmpAllele1(mysqlpp::String allele);
		void setTmpAllele2(mysqlpp::String allele);
		//Members for quality flag
		std::string getQualFlag();
		void setQualFlag(std::string flag);
	private:
		int coordinate;
		int tag_coordinate;
		int tag;
		int offset;
		std::string strand;
		std::string chr;
		std::string ref_allele;
		std::string alt_allele;
		std::string tmp_allele1;
		std::string tmp_allele2;
		std::string qualFlag;
};
#endif   /* ----- #ifndef SNP_H_INC  ----- */
