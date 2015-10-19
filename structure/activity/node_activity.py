####
##
## Take as input the layers of a multiplex, and provide as output a
## file where the n-th line contains the activity of the n-th node. We
## assume that nodes are numbered sequentially, starting from 0, with
## no gaps (i.e., missing nodes are treated as isolated nodes)
##
##
##


import sys

if len(sys.argv) < 2:
    print "Usage: %s <layer1> [<layer2>...]" % sys.argv[0]
    sys.exit(1)

node_activity = {}

max_N = -1

for layer in sys.argv[1:]:
    active = []
    with open(layer, "r") as lines:
        for l in lines:
            if l[0] == "#":
                continue
            
            s, d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
            active.extend([s,d])
            if s > max_N:
                max_N = s
            if d > max_N:
                max_N = d
        active = set(active)
        for n in active:
            if n in node_activity:
                node_activity[n] += 1
            else:
                node_activity[n] = 1
    

for n in range(max_N+1):
    if n in node_activity:
        print node_activity[n]
    else:
        print 0




