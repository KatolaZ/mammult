####
## 
## Aggregate the layers of a multiplex, weigthing each edge according
## to the number of times it occurs in the different layers
##

import sys
import networkx as net


if len(sys.argv) < 3:
    print "Usage: %s <file1> <file2> [<file3>....]" % sys.argv(0)
    sys.exit(1)

G = net.Graph()

lines = open(sys.argv[1], "r").readlines()

for l in lines:
    s,d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
    G.add_edge(s,d)
    G[s][d]['weigth'] = 1

for f in sys.argv[2:]:
    lines = open(f, "r").readlines()
    for l in lines:
        s,d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
        if (G.has_edge(s,d)):
            G[s][d]['weigth'] += 1
        else:
            G.add_edge(s,d)
            G[s][d]['weigth'] = 1

for s,d in G.edges():
    print s,d, G[s][d]['weigth']
        

