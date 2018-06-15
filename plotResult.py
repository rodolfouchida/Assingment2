#!/usr/bin/env python3

# required pip3 install matplotlib
import math
import csv

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


if __name__ == '__main__':
        from mpl_toolkits.mplot3d import Axes3D
        import matplotlib.pyplot as plt
        from matplotlib import cm
        from matplotlib.ticker import LinearLocator, FormatStrFormatter
        import numpy as np


        fig = plt.figure()
        ax = fig.gca(projection='3d')

        # Make data.
        X = np.arange(-2, 2, 0.25)
        Y = np.arange(-2, 2, 0.25)
        X, Y = np.meshgrid(X, Y)
    
        c = readFile("./results.dat")
        m = int(math.sqrt(len(c)) - 1)

        xs, ys, zs = readScatter("./binormal.txt")
        
        Z = []
        for w in range(0, len(xs)):
            s=0
            k=0
            for i in range(0, m):
                for j in range(0, m):
                    s += c[k] * pow(xs[w], i) * pow(ys[w], j)
                    k += 1
            Z.append(s)

        ax.scatter(xs, ys, zs)
        ax.scatter(xs, ys, Z)
        # Customize the z axis.
        ax.set_zlim(-2.01, 2000.01)
        ax.zaxis.set_major_locator(LinearLocator(10))
        ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

        # Add a color bar which maps values to colors.
        #fig.colorbar(surf, shrink=0.5, aspect=5)

        plt.show()
