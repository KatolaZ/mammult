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
## Make a cartography (i.e., sum and participation coefficient) based
## on the values found at the given column numbers of the file given
## as input, e.g.:
##
##    cartography_cols.py FILEIN 1 5 8 14
##
## makes the assumption that the system has 4 layers, and that the
## quantities involved in the cartography are at the 2nd, 6th, 9th and
## 15th column of FILEIN
## 
##
##

import sys

if len(sys.argv) < 4:
    print "Usage: %s <filein> <col1> <col2> [<col3> <col4>...]" % sys.argv[0]
    sys.exit(1)

filein=sys.argv[1]
cols = [int(x) for x in sys.argv[2:]]

M = len(cols)

with open(filein,"r") as f:
    for l in f:
        if l[0] == "#":
            continue
        elems = [float(x) if "e" in x or "." in x else int(x) for x in l.strip(" \n").split(" ")]
        sum_elems = 0
        part = 0
        for c in cols:
            val = elems[c]
            sum_elems += val
            part += val*val
        if sum_elems > 0:
            part = M * 1.0 / (M -1) * (1 - part * 1.0 / (sum_elems * sum_elems))
        else:
            part = 0.0
        print elems[0], sum_elems, part

        
