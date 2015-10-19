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
## Take as input the layers of a multiplex, and provide as output a
## file where the n-th line contains the degrees of the n-th node at
## each layer, separated by a space, in the format:
##
## node1_deglay1 node1_deglay2 .... node1_deglayM
## node2_deglay1 node2_deglay2 .... node2_deglayM
## ..............................................
## nodeN_deglay1 nodeN_deglay2 .... nodeN_deglayM
##
##

import sys

if len(sys.argv) < 2:
    print "Usage: %s <layer1> [<layer2>...]" % sys.argv[0]
    sys.exit(1)

node_degrees = {}

max_N = -1

num_layer = 0

for layer in sys.argv[1:]:
    with open(layer, "r") as lines:
        for l in lines:
            if l[0] == "#":
                continue
            
            s, d = [int(x) for x in l.strip(" \n").split(" ")[:2]]

            if s > max_N:
                max_N = s
            if d > max_N:
                max_N = d

            if s in node_degrees:
                if num_layer in node_degrees[s]:
                    node_degrees[s][num_layer] += 1
                else:
                    node_degrees[s][num_layer] = 1
            else:
                node_degrees[s] = {}
                node_degrees[s][num_layer] = 1

            if d in node_degrees:
                if num_layer in node_degrees[d]:
                    node_degrees[d][num_layer] += 1
                else:
                    node_degrees[d][num_layer] = 1
            else:
                node_degrees[d] = {}
                node_degrees[d][num_layer] = 1
    num_layer += 1


for n in range(max_N+1):
    for i in range(num_layer):
        if n in node_degrees:
            if i in node_degrees[n]:
                print 1,
            else:
                print 0,
        else:
            print 0,
    print
