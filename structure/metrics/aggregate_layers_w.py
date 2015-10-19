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
## Aggregate the layers of a multiplex, weigthing each edge according
## to the number of times it occurs in the different layers
##

import sys
import networkx as net


if len(sys.argv) < 3:
    print "Usage: %s <file1> <file2> [<file3>....]" % sys.argv(0)
    sys.exit(1)

G = net.Graph()

lines = open(sys.argv[1], "r").readlines()

for l in lines:
    s,d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
    G.add_edge(s,d)
    G[s][d]['weigth'] = 1

for f in sys.argv[2:]:
    lines = open(f, "r").readlines()
    for l in lines:
        s,d = [int(x) for x in l.strip(" \n").split(" ")[:2]]
        if (G.has_edge(s,d)):
            G[s][d]['weigth'] += 1
        else:
            G.add_edge(s,d)
            G[s][d]['weigth'] = 1

for s,d in G.edges():
    print s,d, G[s][d]['weigth']
        

