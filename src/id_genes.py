#!/usr/bin/python
# -*- coding: utf-8 -*-
# SKT 09/2013
# Input:
#       (a) output file from plotSites.py - list RE sites that a different b/w
#           tissue types
#       (b) Sbicolor_79_gene.gff3 - genes and position
#       (c) Sbicolor_79_annotation_info.txt - annotation and gene
# Output:
#       (a) file that containes genes with differential me states at their
#           promoters

from pylab import *
import matplotlib.pyplot as plt
from operator import itemgetter
import scipy.stats as sp
from collections import defaultdict
import numpy as np
import sys

def consume(iterator, n):
    collections.deque(itertools.islice(iterator, n))

if len(sys.argv) <= 3:
    print("id_genes.py input.tsv gene.gff3 annotation.txt")
    sys.exit()
if sys.argv[1] == "--help" or sys.argv[1] == "-h":
    print("id_genes.py input.tsv gene.gff3 annotation.txt")
    sys.exit()
try:
    # read in input.tsv
    input_li_tsv = [line.strip() for line in open(sys.argv[1])]
    input_li_tsv = [element.split('\t') for element in input_li_tsv]
    # read in gene.gff3
    gff3_li_tsv = [line.strip() for line in open(sys.argv[2])]
    gff3_li_tsv = [element.split('\t') for element in gff3_li_tsv]
    # read in annotation.txt
    ann_li_tsv = [line.strip() for line in open(sys.argv[3])]
    ann_li_tsv = [element.split('\t') for element in ann_li_tsv]

except IOError:
    print("Cannot open target file. Please check your input:")
    print("\t$ python id_genes.py input.tsv gene.gff3 annotation.txt")
    sys.exit()

# write input into input_li_tsv
li_tsv_temp=[]
li_tsv_filtered=[]
for i in range(len(input_li_tsv)):
    if i == 0:
        continue # header
    for j in range(len(input_li_tsv[i])):
        li_tsv_temp.append(float(input_li_tsv[i][j]))
    li_tsv_filtered.append(li_tsv_temp)
    li_tsv_temp=[]
input_li_tsv = li_tsv_filtered

# write gff3 gene information into gff3_li_tsv
li_tsv_temp=[]
li_tsv_filtered=[]

for i in range(len(gff3_li_tsv)):
    if i == 0:
        continue # header
    if gff3_li_tsv[i][2] == str("gene"):
        li_tsv_temp.append(float((gff3_li_tsv[i][0].split("_"))[1]))# chromosome
        li_tsv_temp.append(float(gff3_li_tsv[i][3]))                # start bp
        li_tsv_temp.append(float(gff3_li_tsv[i][4]))                # end bp
        li_tsv_temp.append(gff3_li_tsv[i][6])                       # +/-
        li_tsv_temp.append((((gff3_li_tsv[i][8].split(";"))[0]).split("="))[1])
        li_tsv_filtered.append(li_tsv_temp)
        li_tsv_temp=[]
gff3_li_tsv = li_tsv_filtered

# write annotation information into a_li_tsv
li_tsv_temp=[]
li_tsv_filtered=[]

for i in range(len(ann_li_tsv)):
    li_tsv_temp.append((ann_li_tsv[i][0].split("."))[0]) # Sbi ID
    for j in range(len(ann_li_tsv[i])):
        if j <= 5: continue
        li_tsv_temp.append(ann_li_tsv[i][j])
    li_tsv_filtered.append(li_tsv_temp)
    li_tsv_temp=[]
ann_li_tsv = li_tsv_filtered

# concatenate a list of annotations for regions upstream of a promoter
diff_me_li = []
# 500 bp upstream a promoter region
bp_upstream = 300

for i in range (len(input_li_tsv)):
    for j in range(len(gff3_li_tsv)):
        if input_li_tsv[i][0] == gff3_li_tsv[j][0] and gff3_li_tsv[j][3]=='+':
            if (input_li_tsv[i][1] <= gff3_li_tsv[j][1]) and (input_li_tsv[i][1] >= (gff3_li_tsv[j][1] - bp_upstream)):
                li_tsv_temp.append(gff3_li_tsv[j][4])
                li_tsv_temp.append(float(gff3_li_tsv[j][1]-input_li_tsv[i][1]))
                for k in range(len(input_li_tsv[i])):
                    if k <= 1: continue
                    li_tsv_temp.append(input_li_tsv[i][k])
                for m in range(len(ann_li_tsv)):
                    if (gff3_li_tsv[j][4]==ann_li_tsv[m][0]):
                        for n in range(len(ann_li_tsv[m])):
                            if n==0: continue
                            li_tsv_temp.append(ann_li_tsv[m][n])
                diff_me_li.append(li_tsv_temp)
                li_tsv_temp=[]
        if input_li_tsv[i][0] == gff3_li_tsv[j][0] and gff3_li_tsv[j][3]=='-':
            if (input_li_tsv[i][1] >= gff3_li_tsv[j][2]) and (input_li_tsv[i][1] <=(gff3_li_tsv[j][2] + bp_upstream)):
                li_tsv_temp.append(gff3_li_tsv[j][4])
                li_tsv_temp.append(float(input_li_tsv[i][1]-gff3_li_tsv[j][2]))
                for k in range(len(input_li_tsv[i])):
                    if k <= 1: continue
                    li_tsv_temp.append(input_li_tsv[i][k])
                for m in range(len(ann_li_tsv)):
                    if (gff3_li_tsv[j][4]==ann_li_tsv[m][0]):
                        for n in range(len(ann_li_tsv[m])):
                            if n==0: continue
                            li_tsv_temp.append(ann_li_tsv[m][n])

                diff_me_li.append(li_tsv_temp)
                li_tsv_temp=[]

#for i in range(len(diff_me_li)):
#    print((diff_me_li[i]))

print '\n'.join(('\t'.join(str(i) for i in item[0:])) for item in diff_me_li)


