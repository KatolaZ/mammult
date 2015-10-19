####
##
## Take as input two files, whose n^th line contains the ranking of
## element n, and compute the Kendall's \tau_b rank correlation
## coefficient
##
##

import sys
from numpy import *


def kendalltau(x,y):
    initial_sort_with_lexsort = True # if True, ~30% slower (but faster under profiler!) but with better worst case (O(n log(n)) than (quick)sort (O(n^2))
    n = len(x)
    temp = range(n) # support structure used by mergesort
    # this closure recursively sorts sections of perm[] by comparing 
    # elements of y[perm[]] using temp[] as support
    # returns the number of swaps required by an equivalent bubble sort
    def mergesort(offs, length):
        exchcnt = 0
        if length == 1:
            return 0
        if length == 2:
            if y[perm[offs]] <= y[perm[offs+1]]:
                return 0
            t = perm[offs]
            perm[offs] = perm[offs+1]
            perm[offs+1] = t
            return 1
        length0 = length / 2
        length1 = length - length0
        middle = offs + length0
        exchcnt += mergesort(offs, length0)
        exchcnt += mergesort(middle, length1)
        if y[perm[middle - 1]] < y[perm[middle]]:
            return exchcnt
        # merging
        i = j = k = 0
        while j < length0 or k < length1:
            if k >= length1 or (j < length0 and y[perm[offs + j]] <= y[perm[middle + k]]):
                temp[i] = perm[offs + j]
                d = i - j
                j += 1
            else:
                temp[i] = perm[middle + k]
                d = (offs + i) - (middle + k)
                k += 1
            if d > 0:
                exchcnt += d;
            i += 1
        perm[offs:offs+length] = temp[0:length]
        return exchcnt
    
    # initial sort on values of x and, if tied, on values of y
    if initial_sort_with_lexsort:
        # sort implemented as mergesort, worst case: O(n log(n))
        perm = lexsort((y, x))
    else:
        # sort implemented as quicksort, 30% faster but with worst case: O(n^2)
        perm = range(n)
        perm.sort(lambda a,b: cmp(x[a],x[b]) or cmp(y[a],y[b]))
    
    # compute joint ties
    first = 0
    t = 0
    for i in xrange(1,n):
        if x[perm[first]] != x[perm[i]] or y[perm[first]] != y[perm[i]]:
            t += ((i - first) * (i - first - 1)) / 2
            first = i
    t += ((n - first) * (n - first - 1)) / 2
    
    # compute ties in x
    first = 0
    u = 0
    for i in xrange(1,n):
        if x[perm[first]] != x[perm[i]]:
            u += ((i - first) * (i - first - 1)) / 2
            first = i
    u += ((n - first) * (n - first - 1)) / 2
    
    # count exchanges 
    exchanges = mergesort(0, n)
    # compute ties in y after mergesort with counting
    first = 0
    v = 0
    for i in xrange(1,n):
        if y[perm[first]] != y[perm[i]]:
            v += ((i - first) * (i - first - 1)) / 2
            first = i
    v += ((n - first) * (n - first - 1)) / 2
    
    tot = (n * (n - 1)) / 2
    if tot == u and tot == v:
        return 1    # Special case for all ties in both ranks
    
    tau = ((tot-(v+u-t)) - 2.0 * exchanges) / (sqrt(float(( tot - u )) * float( tot - v )))
    
    # what follows reproduces ending of Gary Strangman's original stats.kendalltau() in SciPy
    svar = (4.0*n+10.0) / (9.0*n*(n-1))
    z = tau / sqrt(svar)
    ##prob = erfc(abs(z)/1.4142136)
    ##return tau, prob
    return tau

def main():

    if len(sys.argv) < 3:
        print "Usage: %s <file1> <file2>" % sys.argv[0]
        sys.exit(1)

    x1 = []
    x2= []

    lines = open(sys.argv[1]).readlines()

    for l in lines:
        elem = [float(x) if "e" in x or "." in x else int(x) for x in l.strip(" \n").split()][0]
        x1.append(elem)

    lines = open(sys.argv[2]).readlines()

    for l in lines:
        elem = [float(x) if "e" in x or "." in x else int(x) for x in l.strip(" \n").split()][0]
        x2.append(elem)
    

    tau = kendalltau(x1,x2)
    print tau


if  __name__ == "__main__":
    main()
