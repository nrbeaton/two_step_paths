#!/usr/bin/env sage

# USAGE
# ./seriestest.py filename
# where filename has the series, one number per line, starting at length 0

from sage.all import *

from ore_algebra import *

with open(sys.argv[1]) as f:
    lines = f.read().splitlines()

ints = [int(t) for t in lines]

# since there are many more non-d-finite than d-finite things
# it makes sense to check for that first
try:
    # first check for DE
    DE = guess(ints, OreAlgebra(ZZ['x'], 'Dx'))
    try:
        # if that succeeded then check for AE
        AE = guess(ints, OreAlgebra(ZZ['x'], 'C'))
        print(2)
    except ValueError:
        print(1)
except ValueError:
    print(0)
