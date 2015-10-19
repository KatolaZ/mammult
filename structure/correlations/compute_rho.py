####
##
## Get two rankings and a pairing, and compute the corresponding
## Spearman's rho rank correlation coefficient
##
##
##
##

import sys
import random
import math
#import scipy.stats

##
## Compute the constant C of the Spearman's rho correlation coefficient in the draft
## 
def compute_C(rank1, rank2):
    N = len(rank1)
    [mu1, mu2] = [1.0 * sum(x.values()) / len(x.values()) for x in [rank1, rank2]]
    [sum1, sum2] = [1.0 * sum(x.values()) for x in [rank1, rank2]]
    C = N * mu1 * mu2 - mu2 * sum1 - mu1 * sum2

    #print mu1, mu2, sum1, sum2, C

    return C


##
## Compute the constant D of the Spearman's rho correlation coefficient in the draft
## 
def compute_D(rank1, rank2):
    [mu1, mu2] = [1.0 * sum(x.values()) / len(x.values()) for x in [rank1, rank2]]
    s1 = sum([math.pow((x-mu1), 2) for x in rank1.values()])
    s2 = sum([math.pow((x-mu2), 2) for x in rank2.values()])
    
    D = math.sqrt(s1 * s2)
    return D

def compute_rho(rank1, rank2, pairing, C, D):

    rho = 0
    for s,t in pairing:
        rho += rank1[s] * rank2[t]
    rho = (rho + C) / D
    return rho

if len(sys.argv) < 3:
    print "Usage: %s <rank1> <rank2> [<pairing>]" % sys.argv[0]
    sys.exit(1)

rank1 = {}
rank2 = {}

lines = open(sys.argv[1], "r").readlines()

i = 0
for l in lines:
    if l[0] == "#" or l.strip(" \n").split(" ") == []: ## Strip comments and empty lines
        continue
    r = [float(x) if "." in x or "e" in x else int(x) for x in l.strip(" \n").split(" ")][0]
    rank1[i] = r
    i += 1


lines = open(sys.argv[2], "r").readlines()

i = 0
for l in lines:
    if l[0] == "#" or l.strip(" \n").split(" ") == []: ## Strip comments and empty lines
        continue
    r = [float(x) if "." in x or "e" in x else int(x) for x in l.strip(" \n").split(" ")][0]
    rank2[i] = r
    i += 1


N1 = len(rank1)
N2 = len(rank2)

if (N1 != N2):
    print "The two files must have the same number of nodes!!!!!"
    sys.exit(2)



C = compute_C(rank1, rank2)
D = compute_D(rank1, rank2)


## We start from a random pairing, and compute the corresponding value of rho

pairing = []

if len(sys.argv) > 3:
    lines = open(sys.argv[3], "r").readlines()
    
    

    for l in lines:
        if l[0] == "#" or l.strip(" \n").split(" ") == []:
            continue
        p1, p2 = [int(x) for x in l.strip(" \n").split(" ")][:2]
        pairing.append((p1, p2))

else:
    for i in range (N1):
        pairing.append((i,i))


if len(pairing) != N1:
    print "ERROR !!! The pairing should be of the same length of the ranking files!!!"
    sys.exit(1)

rho = compute_rho(rank1, rank2, pairing, C, D)

print rho


