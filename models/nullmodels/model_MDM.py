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
## This is the vertical participation model. For each node i, we use
## exactly the same value of B_i as in the original network, but we
## choose at random the layers in which node i will be active. This
## breaks down intra-layer correlations. 
##
## We get as input a file which reports, for each value of B_i, the
## number of nodes in the original network which have that value, i the format:
##
## B_i N(B_i)
##
##
##
## The output is the obtained distribution of bit-strings.
##
##

import sys
import random


def to_binary(l):
    s = 0
    e = 0
    for v in l:
        s += v * pow(2,e)
        e +=1
    return s


if len(sys.argv) < 3:
    print "Usage: %s <Bi_file> <M>" % sys.argv[0]
    sys.exit(1)

M = int(sys.argv[2])

layers = range(M)

distr = {}

with open(sys.argv[1], "r") as f:
    for l in f:
        if l[0] == "#":
            continue
        val, num = [int(x) for x in l.strip(" \n").split(" ")]
        for j in range(num):
            node_layers = random.sample(layers, val)
            node_bitstring = [0 for x in range(M)]
            #print node_bitstring, node_layers
            for i in node_layers:
                #print i,
                node_bitstring[i] = 1
                #print node_bitstring
                
            bs = to_binary(node_bitstring)
            if bs in distr:
                distr[bs] += 1
            else:
                distr[bs] = 1

for k in distr:
    bin_list = bin(k)
    bin_num = sum([int(x) if x=='1' else 0 for x in bin_list[2:]])
    sys.stderr.write("%d %0175s %d \n" % (bin_num, bin_list[2:], distr[k]))
