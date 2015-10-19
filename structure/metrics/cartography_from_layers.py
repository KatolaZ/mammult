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

    
    
