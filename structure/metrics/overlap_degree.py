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
