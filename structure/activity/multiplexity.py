####
##
## Take as input the layers of a multiplex, and provide as output a
## file where the n-th line contains the activity of the n-th
## layer. 
##
##


import sys

if len(sys.argv) < 3:
    print "Usage: %s <layer1> <layer2> [<layer3>...]" % sys.argv[0]
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

for i in range(len(layers)):
    for j in range(i+1, len(layers)):
        s = layer[i] & layer[j]
        print i, j, len(s)*1.0 / (max_N+1)

