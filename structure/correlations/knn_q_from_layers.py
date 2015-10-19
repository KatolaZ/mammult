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
##
## Compute the degree-degree correlations of a multiplex graph, namely:
##
## <k_1>(k_2) and <k_2>(k_1)
##
## Takes as input the two lists of edges corresponding to each layer
##

import sys
import numpy as np
import networkx as net


def knn(G, n):
    neigh = G.neighbors(n)
    l = G.degree(neigh).values()
    return 1.0 * sum(l) / len(l) 


if len(sys.argv) < 2:
    print "Usage: %s <layer1> <layer2>" % sys.argv[0]
    sys.exit(1)


G1 = net.read_edgelist(sys.argv[1])
G2 = net.read_edgelist(sys.argv[2])

k1_k1 = {} ## Intraleyer knn (k1)
k2_k2 = {} ## Intralayer knn (k2)
k1_k2 = {} ## Interlayer average degree at layer 1 of a node having degree k_2 in layer 2
k2_k1 = {} ## Interlayer average degree at layer 2 of a node having degree k_1 in layer 1


for n in G1.nodes():
    k1 = G1.degree(n)

    
    ##print k1,k2

    knn1 = knn(G1, n)
    if n in G2.nodes():
        k2 = G2.degree(n)
        knn2 = knn(G2, n)
    else:
        k2 = 0
        knn2 = 0

    if k1_k1.has_key(k1):
        k1_k1[k1].append(knn1)
    else:
        k1_k1[k1] = [knn1]

    if k2_k2.has_key(k2):
        k2_k2[k2].append(knn2)
    else:
        k2_k2[k2] = [knn2]
        

    if k1_k2.has_key(k2):
        k1_k2[k2].append(k1)
    else:
        k1_k2[k2] = [k1]

    if k2_k1.has_key(k1):
        k2_k1[k1].append(k2)
    else:
        k2_k1[k1] = [k2]
        
        
k1_keys = k1_k1.keys()
k1_keys.sort()
k2_keys = k2_k2.keys()
k2_keys.sort()


f = open("%s_%s_k1" % (sys.argv[1], sys.argv[2]), "w+")

for n in k1_keys:
    avg_knn = np.mean(k1_k1[n])
    std_knn = np.std(k1_k1[n])
    avg_k2 = np.mean(k2_k1[n])
    std_k2 = np.std(k2_k1[n])
    f.write("%d %f %f %f %f\n" % (n, avg_knn, std_knn, avg_k2, std_k2))

f.close()

f = open("%s_%s_k2" % (sys.argv[1], sys.argv[2]), "w+")

for n in k2_keys:
    avg_knn = np.mean(k2_k2[n])
    std_knn = np.std(k2_k2[n])
    avg_k1 = np.mean(k1_k2[n])
    std_k1 = np.std(k1_k2[n])
    f.write("%d %f %f %f %f\n" % (n, avg_knn, std_knn, avg_k1, std_k1))
f.close()

