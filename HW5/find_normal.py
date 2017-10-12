import numpy as np
from numpy import linalg as LA
import argparse
import os
import sys
A = []
B =[]
C = []

def main(argv):
    '''
    if len(argv) != 2:
        sys.exit("usage: find_normal.py <input.py>")
    
    # Open input file for reading
    with open(argv[1], 'r') as myfile:
        data=myfile.read()
    print type(data)
    
    for i in range(0, len(data)):
		if i < 3:
			if data[i] == '-':
				i +=1
				tmp = int(data[i])
				tmp = -tmp
				A.append(tmp)
			else:
				tmp = int(data[i])
				A.append(tmp)
		if i>=3 and i<6:
			if data[i] == '-':
				i +=1
				tmp = int(data[i])
				tmp = -tmp
				B.append(tmp)
			else:
				tmp = int(data[i])
				B.append(tmp)
		if i>=6 and i<9:
			if data[i] == '-':
				i +=1
				tmp = int(data[i])
				tmp = -tmp
				C.append(tmp)
			else:
				tmp = int(data[i])
				C.append(tmp)
				'''
    P = np.array([2,0,0])
    Q = np.array([1,.5,.5])
    R = np.array([1,-.5,.5])
    
    print('P={}'.format(P))
    print('Q={}'.format(Q))
    print('R={}'.format(R))
    PQ = np.subtract(Q, P)
    PR = np.subtract(R,P)
    print(PQ)
    print(PR)
    normal = np.cross(PQ,PR)
    print('the normal is {}'.format(normal))
    n = LA.norm(normal)
    new_norm = normal/n
    normal = normal/LA.norm(normal)
    print ('are these {} the same {}'.format(new_norm, normal))

if __name__ =='__main__':
	main(sys.argv)
