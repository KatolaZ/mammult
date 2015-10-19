####
##
##
## Take a file with the degree sequence of nodes which are active on
## both layers, and compute \overline{k}(q) and \overline{q}(k),
## i.e. the two inter-layer correlation functions, where we call "k"
## the degree on the first column, and "q" the degree on the second
## column
##
##

import sys

if len(sys.argv) < 2:
    print "Usage: %s <filein>" % sys.argv[0]
    sys.exit(1)

qnn_k = {}
knn_q = {}

with open(sys.argv[1]) as f:
    for l in f:
        k, q = [int(x) for x in l.strip(" \n").split(" ")]
        if qnn_k.has_key(k):
            qnn_k[k].append(q)
        else:
            qnn_k[k] = [q]
        if knn_q.has_key(q):
            knn_q[q].append(k)
        else:
            knn_q[q] = [k]




keys= qnn_k.keys()
keys.sort()
for k in keys:
    print k, 1.0 * sum(qnn_k[k])/len(qnn_k[k])




keys= knn_q.keys()
keys.sort()
for q in keys:
    sys.stderr.write("%d %f\n" % (q, 1.0 * sum(knn_q[q])/len(knn_q[q])))


