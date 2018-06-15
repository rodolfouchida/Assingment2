#!/usr/bin/env python3
import numpy as np
import os, argparse, sys, platform
import matplotlib.pyplot as plt

mean = [0,0]
cov = [[1,0],[0,1]]

def generate(name):
    a, b = np.random.multivariate_normal(mean, cov, 10000).T
   
    z, x, y = np.histogram2d(a, b, bins=10)
   
    print(x)
    print(y)
    print(z)

    extent = [x[0], x[-1], y[0], y[-1]]

    xt = []
    yt = []
    for i in range(0, len(x)-1):
        xt.append(x[i] + (x[i+1]-x[i])/2)
        yt.append(y[i] + (y[i+1]-y[i])/2)

    zt = []
    for i in z:
        for j in i:
            zt.append(j)

    print(xt)
    print(yt)
    print(zt)

    k = 0
    with open(name, "w+") as f:
        for i in xt:
            for j in yt:
                print("[*] {},{},{}\n".format(i,j,zt[k]))
                f.write("{};{};{}\n".format(i,j,zt[k]))
                k += 1
    f.closed

    plt.imshow(z, extent=extent, interpolation='nearest')
    plt.colorbar()
    plt.show()

if __name__=='__main__':
    #print("[*] Python version: {}".format(sys.version))
    #print("[*] Python version: {}".format(platform.python_version()))
    generate('binormal.txt')
