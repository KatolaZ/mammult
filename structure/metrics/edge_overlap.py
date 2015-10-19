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
## Compute the edge overlap of each edge of the multiplex, i.e. the
## number of times that each edge appears in the multiplex
##
##

import sys


if len(sys.argv) < 2:
    print "Usage: %s <layer1> [<layer2>...]" % sys.argv[0]
    sys.exit(1)

max_N = -1

all_edges = {}

layer_ID = -1

for layer in sys.argv[1:]:
    layer_ID += 1
    with open(layer, "r") as lines:
        for l in lines:
            if l[0] == "#":
                continue
            s, d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
            if s > d:
                tmp = s
                s = d
                d = tmp
            if (s,d) in all_edges:
                all_edges[(s,d)].append(layer_ID)
            else:
                all_edges[(s,d)] = [layer_ID]
                
K = len(all_edges.keys())

for k in all_edges.keys():
    val = len(set(all_edges[(k)]))
    s,d = k
    print s, d, val

