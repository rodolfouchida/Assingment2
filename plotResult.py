#!/usr/bin/env python3

# required pip3 install matplotlib
import math
import csv
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

def readFile(name):
    with open(name, newline='') as csvfile:
        content = csv.reader(csvfile, delimiter=';')
        print(content)
        c = []
        for row in content:
            c.append(float(row[0]))
        return c

def readScatter(name):
    with open(name, newline='') as csvfile:
        content = csv.reader(csvfile, delimiter=';')
        print(content)
        x = []
        y = []
        z = []
        for row in content:
            x.append(float(row[0])) 
            y.append(float(row[1]))
            z.append(float(row[2]))

        return x, y, z

def erro(A, B):
    C = []
    for i,j in zip(A, B):
        C.append(np.sqrt(pow(i-j, 2)))
    print("[*] Erro médio: {}".format(np.mean(C)))
    print("[*] Erro máximo: {}".format(np.max(C)))

if __name__ == '__main__':

        fig = plt.figure()
        ax = fig.gca(projection='3d')

        c = readFile("./results.dat")
        xs, ys, zs = readScatter("./binormal.txt")
        
        n = len(xs)
        m = len(c)
        A = [0] * n
        for i in range(n):
            A[i] = [0] * m

        for w in range(0, len(xs)):
            A[w][0] = 1.0
            A[w][1] = xs[w]
            A[w][2] = ys[w]
            A[w][3] = xs[w]*ys[w]
            for i in range(4, len(c)):
                if(i % 2 == 0):
                    A[w][i] = A[w][i-1]*A[w][1]
                else:
                    A[w][i] = A[w][i-1]*A[w][2]

        Z = []
        for w in range(0, len(xs)):
            s=0
            for i in range(0, len(c)):
                s += c[i] * A[w][i]
            Z.append(s)

        # Make data.
        X = np.arange(0, 1, 0.05)
        Y = np.arange(0, 1, 0.05)
        X, Y = np.meshgrid(X, Y) 
        Zp = c[0] + c[1]*X + c[2]*Y + c[3]*X*Y + c[4]*(X**2)*Y + c[5]*(X**2)*(Y**2) + c[6]*(X**3)*(Y**2) + c[7]*(X**3)*(Y**3) 
        
        ax.scatter(xs, ys, zs)
        ax.scatter(xs, ys, Z)
        ax.plot_surface(X, Y, Zp)

        erro(Z, Zp)

        # Customize the z axis.
        ax.set_zlim(0.01, 1.01)
        ax.zaxis.set_major_locator(LinearLocator(10))
        ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

        # Add a color bar which maps values to colors.
        #fig.colorbar(surf, shrink=0.5, aspect=5)

        plt.show()
