# This file is part of MAMMULT: Metrics And Models for Multilayer Networks
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or (at
# your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
