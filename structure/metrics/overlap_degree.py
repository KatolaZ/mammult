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
## Compute the overlapping degree for each node and the corresponding
## z-score 
##
##

import sys
import numpy


if len(sys.argv) < 2:
    print "Usage: %s <layer1> <layer2> [<layer3>...]" % sys.argv[0]
    sys.exit(1)


nodes = {}

for f in sys.argv[1:]:

    lines = open(f).readlines()

    for l in lines:
        if l[0] == "#":
            continue
        s,d = [int(x) for x in l.strip(" \n").split(" ")]
        if nodes.has_key(s):
            nodes[s] +=1
        else:
            nodes[s] = 1
        if nodes.has_key(d):
            nodes[d] +=1
        else:
            nodes[d] = 1


degrees = nodes.values()
avg_deg = numpy.mean(degrees)
std_deg = numpy.std(degrees)

#print avg_deg, std_deg

keys = nodes.keys()
keys.sort()

for n in keys:
    print n, nodes[n], (nodes[n] - avg_deg)/std_deg
