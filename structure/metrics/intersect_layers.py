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


