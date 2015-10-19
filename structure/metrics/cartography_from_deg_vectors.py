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
## Take as input a file containing, on each line, the degree vector of
## a node of the multiplex, and compute the multiplex cartography
## diagram
##
##

import sys

if len(sys.argv) < 2:
    print "Usage: %s <node_deg_vectors>" % sys.argv[0]
    sys.exit(1)

filein=sys.argv[1]

M = -1

with open(filein,"r") as lines:
    for l in lines:
        if l[0] == "#":
            continue
        elems = [int(x) for x in l.strip(" \n").split(" ")]
        if (M == -1):
            M = len(elems)
        sum_elems = 0
        part = 0
        for val in elems:
            sum_elems += val
            part += val*val
        if sum_elems > 0:
            part = M * 1.0 / (M -1) * (1 - part * 1.0 / (sum_elems * sum_elems))
        else:
            part = 0.0
        print sum_elems, part

        
