####
##
## Compute the pearson correlation coefficient between the values of
## node properties included in the two files provided as input.
##

import sys
import numpy
import scipy.stats
import math

if len(sys.argv) < 3:
    print "Usage %s <file1> <file2>" % sys.argv[0]
    sys.exit(1)


x1 = []

with open(sys.argv[1], "r") as lines:
    for l in lines:
        elem = [float(x) if "." in x or "e" in x else int(x) for x in l.strip(" \n").split()][0]
        x1.append(elem)

x2 = []

with open(sys.argv[2], "r") as lines:
    for l in lines:
        elem = [float(x) if "." in x or "e" in x else int(x) for x in l.strip(" \n").split()][0]
        x2.append(elem)

    
r2 =numpy.corrcoef(x1,x2)
print r2[0][1]
