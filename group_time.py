#!/usr/bin/env sage

# USE:
# ./sagegroup_order.py T D tmax
# where T is the 16-digit bitstring for the matrix
# D is one of E, N, W, S
# and nmax is a positive integer, corresponding to the number of seconds to try the substitutions and factorisation

from sage.all import *
import signal

# the 1001010... string
Tstring = sys.argv[1]

# convert that into the matrix
Tlist = [[0 for i in range(4)] for j in range(4)]
for k in range(16):
    Tlist[k//4][k%4] = int(Tstring[k])

T = matrix(Tlist)

# some matrices and vectors to start up
IE = diagonal_matrix([0,1,1,1])
IN = diagonal_matrix([1,0,1,1])
IW = diagonal_matrix([1,1,0,1])
IS = diagonal_matrix([1,1,1,0])
I4 = identity_matrix(4)

# variables
t,x,y,X,Y = var('t,x,y,X,Y')

# the version of T with t,x,y
Txy = T*diagonal_matrix([t*x,t*y,t/x,t/y])

D = sys.argv[2]

# 1-B is roughly the same as the kernel for a regular quarter plane model
if D is 'E':
    B = ((Txy.T)[0])*(~(I4-IE*(Txy.T))*IE*Txy[0] + (I4-IE)*vector([1,1,1,1]))
elif D is 'N':
    B = ((Txy.T)[1])*(~(I4-IN*(Txy.T))*IN*Txy[1] + (I4-IN)*vector([1,1,1,1]))
elif D is 'W':
    B = ((Txy.T)[2])*(~(I4-IW*(Txy.T))*IW*Txy[2] + (I4-IW)*vector([1,1,1,1]))
elif D is 'S':
    B = ((Txy.T)[3])*(~(I4-IS*(Txy.T))*IS*Txy[3] + (I4-IS)*vector([1,1,1,1]))
else:
    raise ValueError

# finding the group
BX = B.subs(x = X)
BY = B.subs(y = Y)
Xsols = solve(B == BX, X)
Ysols = solve(B == BY, Y)

# getting the generators
if (bool(Xsols[0].right() == x)):
    xp = Xsols[1].right().factor()
else:
    xp = Xsols[0].right().factor()

if (bool(Ysols[0].right() == y)):
    yp = Ysols[1].right().factor()
else:
    yp = Ysols[0].right().factor()

# using signal to set up a timer
def signal_handler(signum, frame):
    raise Exception("Timed out!")

tmax = int(sys.argv[3])

signal.signal(signal.SIGALRM, signal_handler)
signal.alarm(tmax)

try:
    # trying to find the order of the group
    pairs = [(x,y)]
    # we have to assume some upper bound at which we give up
    # and declare the group to be infinite
    n = 1
    while True:
        if n%2 == 1:
            pairs.append((pairs[n-1][0].subs(x=xp).factor(),pairs[n-1][1].subs(x=xp).factor()))
        else:
            pairs.append((pairs[n-1][0].subs(y=yp).factor(),pairs[n-1][1].subs(y=yp).factor()))
        if bool(pairs[n][0] == x) and bool(pairs[n][1] == y):
            break
        n += 1
    print(n)
except:
    print(0)
