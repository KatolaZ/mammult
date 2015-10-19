####
##
## Take a certain number of networks as input and give as output the
## corresponding intersection graph, i.e. the graph in which an edge
## exists between i and j if and only if it exists in ALL the graphs
## given as input
##


import sys


if len(sys.argv)< 3:
    print "Usage: %s <file1> <file2> [<file3>....]" % sys.argv[0]
    sys.exit(1)


graphs = {}

num = 0

for fname in sys.argv[1:]:

    lines = open(fname).readlines()
    graphs[num] = []
    for l in lines:
        s,d = [int(x) for x in l.strip(" \n").split(" ")][:2]
        if s > d:
            graphs[num].append((d,s))
        else:
            graphs[num].append((d,s))
    num += 1

#print graphs


for edge in graphs[0]:
    to_print = True
    for i in range(1, num):
        if edge not in graphs[i]:
            to_print = False
            break
    if to_print:
        s,d = edge
        print s,d


