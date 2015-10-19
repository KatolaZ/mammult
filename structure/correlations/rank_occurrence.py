####
##
## Get two rankings and compute the size of the k-intersection,
## i.e. the number of elements which are present in the first k
## positions of both rankings, as a function of k
##
##


import sys


if len(sys.argv)< 4:
    print "Usage: %s <file1> <file2> <increment>" % sys.argv[0]
    sys.exit(1)

incr = int(sys.argv[3])

rank1 = []
rank2 = []

lines = open(sys.argv[1], "r").readlines()

for l in lines:
    n= l.strip(" \n").split(" ")[0]
    rank1.append(n)

lines = open(sys.argv[2], "r").readlines()

for l in lines:
    n= l.strip(" \n").split(" ")[0]
    rank2.append(n)


N = len(rank1)

i = incr

while i < N+incr:
    l = len(set(rank1[:i]) & set(rank2[:i]))
    print i, l
    i += incr



