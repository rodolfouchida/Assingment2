#!/usr/bin/env python3
import numpy as np
import os, argparse, sys, platform

mean = [0,0,0]
cov = [[1,0,0],[0,1,0],[0,0,1]]

def generate(n):
    x, y, z = np.random.multivariate_normal(mean, cov, n).T
    return list(x), list(y), list(z)

def writeFile(x, y, z, name):
    with open(name, "w+") as f:
        for i, j, k in zip(x, y, z):
            f.write("{};{};{}\n".format(round(i,4), round(j,4), round(k, 4)))
    f.closed

if __name__=='__main__':
    #print("[*] Python version: {}".format(sys.version))
    #print("[*] Python version: {}".format(platform.python_version()))
    x, y, z = generate(5)
    writeFile(x, y, z, 'binormal.txt')
