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
## Get an edgelist, a file pof arrival times and a node ID and return
## the degree of that node as a function of time (we suppose that IDs
## are sequential)
##
##

import sys


if len(sys.argv) < 4:
    print "Usage: %s <netfile> <timefile> <nodeid1> [<nodeid2> <nodeid3>...]" % sys.argv[0]
    sys.exit(1)

node_id = int(sys.argv[3])

lines = open(sys.argv[2], "r").readlines()

arrival_time = {}

#### WARNING!!!! THIS WORKS ONLY FOR M0=3
arrival_time[0] = 0
arrival_time[1] = 1
arrival_time[2] = 2


neigh_by_time = {}

max_t = -1

for l in lines:
    if l[0] == "#":
        continue
    t,node = [int(x) for x in l.strip(" \n").split(" ")]
    arrival_time[node] = t
    if t > max_t :
	max_t = t


lines = open(sys.argv[1], "r").readlines()


for l in lines:
    if l[0] == "#":
        continue
    n1,n2 = [int(x) for x in l.strip(" \n").split(" ")]


    if neigh_by_time.has_key(n1):
        neigh_by_time[n1].append(arrival_time[n2])
    else:
        neigh_by_time[n1] = [arrival_time[n2]]
        
    if neigh_by_time.has_key(n2):
        neigh_by_time[n2].append(arrival_time[n1])
    else:
        neigh_by_time[n2] = [arrival_time[n1]]



#print neigh_by_time[node_id]


for node_id in sys.argv[3:]:
    node_id = int(node_id)
    neigh_by_time[node_id].sort()
    last_time = neigh_by_time[node_id][0]
#### changed here
    k = 1
    print "#### ", node_id
    for t in neigh_by_time[node_id][1:]:
        if t != last_time:
            if last_time < arrival_time[node_id]:
                print arrival_time[node_id], k
            else:
                print last_time, k
            last_time = t
        k += 1
    print max_t, k-1
    print 
    print
