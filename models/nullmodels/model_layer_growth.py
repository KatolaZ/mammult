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
## layer-by-layer multiplex growth
##
## - We start from a multiplex with M_0 layers, with a certain number of 
##   active nodes each
##
## - Each new layer arrives with a certain number N\lay{\alpha} of nodes
##   to be activated (this number is sampled from the observed distribution 
##   of N\lay{\alpha}, like in the airlines multiplex)
## 
## - Each node $i$ is activated with a probability proportional to the
##   number of existing layers in which it is already active, added to an
##   attractivity A :
##
## P_i(t) \propto A + B_i(t)
##
## - the hope is that A might tune the exponent of the resulting distribution
##   of B_i.....
## 
##
## This script takes as input a file which contains the degrees of the
## layers, the total number of nodes in the multiplex, the initial
## number M0 of layers in the multiplex and the attractiveness
## parameter A. If "RND" is specified as a third parameter, then the
## sequence of N\lay{\alpha} is shuffled
##
## Gives as output a list of node-layer participation
##

import sys
import random

if len(sys.argv) < 5:
    print "Usage: %s <layers_N> <N> <M0> <A> [RND]" % sys.argv[0]
    sys.exit(1)

N = int(sys.argv[2])
M0 = int(sys.argv[3])
A = int(sys.argv[4])

layer_degs = []


if len(sys.argv) > 5 and sys.argv[5] == "RND":
    randomize = True
else:
    randomize = False

lines = open(sys.argv[1]).readlines()

M = 0


for l in lines:
    if l[0] == "#":
        continue
    n = [int(x) for x in l.strip(" \n").split(" ")][0]
    layer_degs.append(n)
    M += 1


if randomize:
    random.shuffle(layer_degs)

## the list node_Bi contains, at each time, the attachment
## probabilities, i.e. it is a list which contains each node $i$
## exactly $A + B_i$ times

node_Bi = []

#
# initialize the distribution of attachment proibabilities, giving to
# all nodes an attachment probability equal to A
#

for i in range(N):
    for j in range(A):
        node_Bi.append(i)
        
layers = []


for i in range(M0):
    N_alpha = layer_degs.pop()
    node_list = []
    num_nodes = 0
    while num_nodes < N_alpha:
        val = random.choice(range(N))
        if val not in node_list:
            node_list.append(val)
            num_nodes += 1
    for n in node_list:
        node_Bi.append(n)
    layers.append(node_list)
    i += 1


#sys.exit(1)

while i < M:
    N_alpha = layer_degs.pop()
    node_list = []
    num_nodes = 0
    while num_nodes < N_alpha:
        val = random.choice(node_Bi)
        if val not in node_list:
            node_list.append(val)
            num_nodes += 1
    for n in node_list:
        node_Bi.append(n)
    layers.append(node_list)
    i += 1

#print layers

for i in range(M):
    node_list = layers[i]
    for n in node_list:
        print n, i
    
