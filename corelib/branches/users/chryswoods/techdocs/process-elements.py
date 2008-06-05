#!/bin/env python

#Process 'element.txt' into a data file that can be compiled into ElementDB

import sys

if (len(sys.argv) < 3):
    print "USAGE: process-elements.py element-names.txt element-data.txt > element-data.h"
    sys.exit(0)

#read in the element names
lines = file(sys.argv[1],"r").readlines()
elements = {}
for line in lines:
    words = line.split()
    if len(words) >= 2:
        elements[words[1].lower()] = words[0]
        
elements["xx"] = "dummy"
    
lines = file(sys.argv[2],"r").readlines()

print "void ElementDB::populate()\n{"

for line in lines:
    if len(line) < 1:
        continue
    elif line[0:1] == "#":
        continue
    else:
        words = line.split()
        if len(words) == 13:
            print "this->import( new ElementData(%s, QObject::tr(\"%s\"), \"%s\", %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s) );" \
            % (words[0],elements[words[1].lower()],words[1],words[11], words[12], words[2],words[3],words[4],words[5],words[6], \
               words[7],words[8],words[9],words[10])
        
print "}"

