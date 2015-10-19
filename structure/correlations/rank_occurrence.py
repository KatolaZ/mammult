# This file is part of MAMMULT: Metrics And Models for Multilayer Networks
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or (at
# your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
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



