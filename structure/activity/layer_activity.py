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


for layer in sys.argv[1:]:
    active = []
    with open(layer, "r") as lines:
        for l in lines:
            if l[0] == "#":
                continue
            
            s, d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
            active.extend([s,d])
        active = set(active)
        print len(active)
