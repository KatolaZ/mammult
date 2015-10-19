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
## This is the hypergeometric model. Each layer has the same number of
## non-isolated nodes as the initial graph, but the nodes are
## activated at random. The input is a file of number of non-isolated
## nodes in each layer, and the total number of nodes in the multiplex.
##
## The output file is a node-layer participation file, in the format
##
## node_id1 layer_id1
## node_id2 layer_id2
## .....
##

import sys
import random

if len(sys.argv) < 3:
    print "Usage: %s <layer_N_file> <N>" % sys.argv[0]
    sys.exit(1)

N = int(sys.argv[2])

layer_degs = []
node_layers = {}

lines = open(sys.argv[1]).readlines()

M = 0


for l in lines:
    if l[0] == "#":
        continue
    n = [int(x) for x in l.strip(" \n").split(" ")][0]
    layer_degs.append(n)
    M += 1

for i in range(M):
    num = layer_degs[i]
    added = []
    n = 0
    while n < num:
        j = random.choice(range(N))
        if j not in added:
            n += 1
            added.append(j)
            if node_layers.has_key(j):
                node_layers[j].append(i)
            else:
                node_layers[j] = [i]
        
for n in node_layers.keys():
    for i in node_layers[n]:
        print n,i

