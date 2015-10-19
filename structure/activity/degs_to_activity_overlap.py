####
##
## Take a file which contains, on the n-th line, the degrees at each
## layer of the n-th node, and print on output the corresponding node
## multi-activity (i.e., the number of layers in which the node is
## active) and the overlapping degree (i.e., the total number of edges
## incident on the node)
## 
##


import sys

def to_binary(l):
    s = 0
    e = 0
    for v in l:
        s += v * pow(2,e)
        e +=1
    return s

if len(sys.argv) < 2:
    print "Usage: %s <filein>" % sys.argv[0]
    sys.exit(1)

distr = {}

with open(sys.argv[1]) as f:
    for l in f:
        elems = [int(x) for x in l.strip(" \n").split(" ")]
        ov = sum(elems)
        new_list = [1 if x>0 else 0 for x in elems]
        multi_act = sum(new_list)
        if multi_act and ov:
            print ov, multi_act
    
