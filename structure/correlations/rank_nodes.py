####
##
##
## Get a file as input, whose n-th line corresponds to the value of a
## certain property of node n, and rank nodes according to their
## properties, taking into account ranking ties properly.
##
## The output is a file whose n-th line is the "ranking" of the n-th
## node according to the given property. (notice that rankings could
## be fractional, due to the tie removal algorithm)
##
##


import sys
import math


if len(sys.argv) < 2:
    print "Usage: %s <filein>" % sys.argv[0]
    sys.exit(1)



lines = open(sys.argv[1], "r").readlines()

ranking = []

n=0
for l in lines:
    if l[0] == "#" or l.strip(" \n").split(" ") == []:
        continue
    v = [float(x) if "." in x or "e" in x else int(x) for x in l.strip(" \n").split(" ")][0]
    ranking.append((v,n))
    n +=1

ranking.sort(reverse=True)
#print ranking
new_ranking = {}

v0, n0 = ranking[0]

old_value = v0
tot_rankings = 1

stack = [n0]
l=1.0
for v,n in ranking[1:]:
    l += 1
    ##print stack, tot_rankings
    if v != old_value: ### There is a new rank
        # We first compute the rank for all the nodes in the stack and then we set it
        new_rank_value = 1.0 * tot_rankings / len(stack)
        ##print new_rank_value
        for j in stack:
            new_ranking[j] = new_rank_value
        old_value = v
        tot_rankings = l
        stack = [n]
    else: # One more value with the same rank, keep it for the future
        stack.append(n)
        tot_rankings += l

new_rank_value = 1.0 * tot_rankings / len(stack)
#print new_rank_value
for j in stack:
    new_ranking[j] = new_rank_value

#print new_ranking

keys = new_ranking.keys()
keys.sort()
for k in keys:
    print new_ranking[k]
