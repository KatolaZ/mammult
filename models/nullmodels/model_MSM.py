####
##
##
## Create a synthetic multiplex network in which a node $i$ appears at
## each layer $\alpha$ with a probability equal to $B_i$, which is the
## fraction of layers in which node $i$ participate in the original
## multiplex.
##
## Take a file of node binary participation indices, and sample a
## multiplex compatible with that distribution
##
##
## The input file is in the format:
## 
## nodeID_i B_i
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
    print "Usage: %s <filein> <M>" % sys.argv[0]
    sys.exit(1)

M = int(sys.argv[2])

bin_index = {}
node_ids = []

lines = open(sys.argv[1]).readlines()


for l in lines:
    if l[0] == "#":
        continue
    elems = [int(x) for x in l.strip(" \n").split(" ")]
    bin_index[elems[0]] = 1.0 * elems[1]/M
    node_ids.append(elems[0])

N = len(node_ids)

node_layers = {}

for alpha in range (M):
    for i in node_ids:
        val = random.random()
        if val < bin_index[i]:
            if  node_layers.has_key(i):
                node_layers[i].append(alpha)
            else:
                node_layers[i] = [alpha]


for i in node_ids:
    if node_layers.has_key(i):
        for j in range(len(node_layers[i])):
            print i, node_layers[i][j]

