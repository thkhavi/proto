#!/usr/bin/python
# -*- coding: utf-8 -*-

from pylab import *
import matplotlib.pyplot as plt
from operator import itemgetter
import scipy.stats as sp
from collections import defaultdict
import numpy as np
import sys

def consume(iterator, n):
    collections.deque(itertools.islice(iterator, n))

if len(sys.argv) <= 1:
    print("plotREsites.py input.tsv")
    sys.exit()
if sys.argv[1] == "--help" or sys.argv[1] == "-h":
    print("plotREsites.py input.tsv")
    sys.exit()
try:
    li_tsv = [line.strip() for line in open(sys.argv[1])]
    li_tsv = [element.split(' ') for element in li_tsv]
except IOError:
    print("Cannot open target file. Please check your input:")
    print("\t$ python plotREsites.py input.tsv")
    sys.exit()

#index = (range(len(list_dataFile))).__iter__()
# The line where the first sequence starts
li_tsvFiltered = []
for i in range(len(li_tsv)):
    if li_tsv[i][0] == 'sample_name':
        li_tsvFiltered.append(li_tsv[i])
        continue
    samples_present = 0
    for j in range(len(li_tsv[i])):
        if li_tsv[i][j]== '1':
            samples_present=samples_present+1
    if samples_present == len(li_tsv[i])-1:
        continue
    if len(li_tsv[i][0].split("super")) < 2:
        li_tsvFiltered.append(li_tsv[i])

li_tsv = li_tsvFiltered
li_tsv_temp=[]
li_tsvFiltered=[]

for i in range(len(li_tsv)):
    if li_tsv[i][0] == 'sample_name':
        li_tsvFiltered.append(li_tsv[i])
        continue
    li_tsv_temp.append(float((li_tsv[i][0].split("_"))[1])) # chromosome
    li_tsv_temp.append(float((li_tsv[i][0].split("_"))[2])) # bp position
    for j in range(len(li_tsv[i])):
        if j != 0:
            li_tsv_temp.append(float((li_tsv[i][j]))) # sample j state at site i
    li_tsvFiltered.append(li_tsv_temp)
    li_tsv_temp=[]
li_tsv = li_tsvFiltered

li_1X = []
li_1Y = []
li_2X = []
li_2Y = []
li_3X = []
li_3Y = []
li_4X = []
li_4Y = []
li_5X = []
li_5Y = []
li_6X = []
li_6Y = []
li_7X = []
li_7Y = []
li_8X = []
li_8Y = []


for i in range(len(li_tsv)):
    if li_tsv[i][0] == 'sample_name':
        continue
    if li_tsv[i][2] == 1:
            li_1Y.append(li_tsv[i][0] - 0.3)
            li_1X.append(li_tsv[i][1]/  1000000)
    if li_tsv[i][3] == 1:
            li_2Y.append(li_tsv[i][0] - 0.2)
            li_2X.append(li_tsv[i][1]/  1000000)
    if li_tsv[i][4] == 1:
            li_3Y.append(li_tsv[i][0] - 0.1)
            li_3X.append(li_tsv[i][1]/  1000000)
    if li_tsv[i][5] == 1:
            li_4Y.append(li_tsv[i][0] - 0.0)
            li_4X.append(li_tsv[i][1]/  1000000)
    if li_tsv[i][6] == 1:
            li_5Y.append(li_tsv[i][0] + 0.1)
            li_5X.append(li_tsv[i][1]/  1000000)
    if li_tsv[i][7] == 1:
            li_6Y.append(li_tsv[i][0] + 0.2)
            li_6X.append(li_tsv[i][1]/  1000000)
    if li_tsv[i][8] == 1:
            li_7Y.append(li_tsv[i][0] + 0.3)
            li_7X.append(li_tsv[i][1]/  1000000)
    if li_tsv[i][9] == 1:
            li_8Y.append(li_tsv[i][0] + 0.4)
            li_8X.append(li_tsv[i][1]/  1000000)

font = {'size'   : 10}

matplotlib.rc('font', **font)
p1, = plt.plot(li_1X, li_1Y, 'o', color='#DC143C',
              alpha=0.9, markersize=5, label="imbibed seed")
p2, = plt.plot(li_2X, li_2Y, 'o', color='#FF1493',
              alpha=0.9, markersize=5, label="mesocotyl")
p3, = plt.plot(li_3X, li_3Y, 'o', color='#8B4513',
              alpha=0.9, markersize=5, label="root")
p4, = plt.plot(li_4X, li_4Y, 'o', color='#FF6347',
              alpha=0.9, markersize=5, label="leaf 1")
p5, = plt.plot(li_5X, li_5Y, 'o', color='#FFD700',
              alpha=0.9, markersize=5, label="leaf 3")
p6, = plt.plot(li_6X, li_6Y, 'o', color='#4B0082',
              alpha=0.9, markersize=5, label="leaf 5")
p7, = plt.plot(li_7X, li_7Y, 'o', color='#32CD32',
              alpha=0.9, markersize=5, label="leaf 6")
p8, = plt.plot(li_8X, li_8Y, 'o', color='#4169E1',
              alpha=0.9, markersize=5, label="leaf 8")

plt.yticks(range(1, 11))  #, ylabels)
plt.ylabel("Chromosome")
plt.xlabel("Position (Mbp)")
plt.title("methylation sites for differential BTx623 tissue")
str_seed = "imbibed seed (" + str(len(li_1X)) + ")"
str_meso = "mesocotyl (" + str(len(li_2X)) + ")"
str_root = "root (" + str(len(li_3X)) + ")"
str_l1 = "leaf 1 (" + str(len(li_4X)) + ")"
str_l3 = "leaf 2 (" + str(len(li_5X)) + ")"
str_l5 = "leaf 5 (" + str(len(li_6X)) + ")"
str_l6 = "leaf 6 (" + str(len(li_7X)) + ")"
str_l8 = "leaf 8 (" + str(len(li_8X)) + ")"


print '\n'.join(('\t'.join(str(i) for i in item[0:])) for item in li_tsv)

plt.legend((p1, p2, p3, p4, p5, p6, p7, p8
            ), (str_seed, str_meso, str_root,
    str_l1, str_l3, str_l5, str_l6, str_l8
                ), numpoints=7, frameon=False, markerscale=.5)
plt.ylim(0.5, 10.5)
plt.xlim(0, 85)
plt.show()

