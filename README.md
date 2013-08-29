stacksToVCF
===========

Generates a VCF of reliable SNP from a Stacks MySQL database and reference genome indexed by Biopieces


This Makefiles contained in the src directory are tailored for either 32-bit or 64-bit Linux Ubuntu OS where the MySQL C API (mysqlclient) has been installed via the package manager, and the MySQL C++ API (mysql++) has been installed by compiling from source and then installing (see this site for additional pointers http://codextechnicanum.blogspot.com/2013/08/compile-mysql-linux-ubuntu-32-bit.html). As such, you may need to change the Makefile to designate the locations of the header files and libraries for your system.

This also requires Biopieces (http://code.google.com/p/biopieces/) be present in the PATH variable, as it makes an external call to the get_genome_seq biopiece to pull information from an indexed reference genome (additional pointers on installing Biopieces can be found here: http://codextechnicanum.blogspot.com/2013/07/installation-of-biopieces-on-ubuntu.html). The name of the reference genome is currently hard-coded and would need to be changed for use on other biopieces indexed reference genomes.

Typical use case:

     stacksToVCF -d databaseName -s localhost -u root > outfile.vcf
     
Known issues:

Stacks appears to miscalcuate coordinates when an indel is present in a tag. SNPs contained in tags with indels cannot be accurately located at this time.
