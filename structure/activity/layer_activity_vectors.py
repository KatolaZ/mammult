####
##
## Take as input the layers of a multiplex, and provide as output a
## file where the n-th line contains the activity of the n-th
## layer. 
##
##


import sys

if len(sys.argv) < 2:
    print "Usage: %s <layer1> [<layer2>...]" % sys.argv[0]
    sys.exit(1)

max_N = -1

layers = []


for layer in sys.argv[1:]:
    active = []
    with open(layer, "r") as lines:
        for l in lines:
            if l[0] == "#":
                continue
            
            s, d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
            if s > max_N:
                max_N = s
            if d > max_N:
                max_N = d
            active.extend([s,d])
        active = set(active)
        layers.append(active)

for l in layers:
    for n in range(max_N+1):
        if n in l:
            print 1,
        else:
            print 0,
    print

