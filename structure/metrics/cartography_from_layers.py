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
## Compute the participation coefficient of each node of a multiplex
## 
##

import sys
import networkx as net
import collections
from compiler.ast import flatten



if len(sys.argv) < 3:
    print "Usage: %s <layer1> <layer2> [<layer3>...]" % sys.argv[0]
    sys.exit(1)


layers = []

for f in sys.argv[1:]:
    G = net.Graph(net.read_edgelist(f, nodetype=int))
    layers.append(G)

nodes = flatten([x for j in layers for x in j.nodes()])
#nodes.sort()
nodes = set(nodes)

M = len(layers)

#print nodes

for n in nodes:
    deg_alpha_square = 0
    deg = 0
    col = 0
    print n, 
    for l in layers:
        val = l.degree([n])
        if not val:
            col = 2* col
            continue
        col *= 2
        col += 1
        val = val[n]
        deg += val
        deg_alpha_square += val*val
    if deg > 0:
        print  deg, 1.0 * M / (M-1) * (1.0 - 1.0 * deg_alpha_square/(deg * deg)) , col
    else:
        print  0 , 0, col

    
    
