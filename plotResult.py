#!/usr/bin/env python3

# required pip3 install matplotlib

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
        X = np.arange(-5, 5, 0.25)
        Y = np.arange(-5, 5, 0.25)
        X, Y = np.meshgrid(X, Y)
    
        c = readFile("./results.dat")
        Z = c[0] + c[1]*X + c[2]*Y + c[3]*X*Y
        for i in range(4, len(c)):
            if i % 2 == 0:
                Z += c[i]*pow(X, (i/2))*pow(Y, (i/2)-1)
            else:
                Z += c[i]*pow(X, (i-1)/2)*pow(Y, (i-1)/2)

        # Plot the surface.
        surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=False)

        xs, ys, zs = readScatter("./binormal.txt")
        ax.scatter(xs, ys, zs)
        
        # Customize the z axis.
        ax.set_zlim(-5.01, 5.01)
        ax.zaxis.set_major_locator(LinearLocator(10))
        ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

        # Add a color bar which maps values to colors.
        fig.colorbar(surf, shrink=0.5, aspect=5)

        plt.show()
