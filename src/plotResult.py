#!/usr/bin/env python3

# required pip3 install matplotlib
import os, glob, math, csv, itertools
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

# Propriedades da distribuição binormal
#   x = sigma1, mu1
#   y = sigma2, mu2
#   rho = ?
sigma1 = 0.5 
sigma2 = 0.5 
mu1 = 0.5 
mu2 = 0.5 
rho = 0 

def f(x,y):
    tmp1 = 2*math.pi*sigma1*sigma2*np.sqrt(1-pow(rho,2))
    tmp2 = np.exp(-Q(x,y))
    return (1/tmp1)*tmp2

def Q(x,y):
    tmp1 = x-mu1
    tmp2 = y-mu2
    tmp3 = 1-pow(rho,2)
    return (1/2*tmp3)*( pow(tmp1/sigma1,2) - 2*rho*(tmp1/sigma1)*(tmp2/sigma2) + pow(tmp2/sigma2, 2) )

def readFile(name):
    with open(name, newline='') as csvfile:
        content = csv.reader(csvfile, delimiter=';')
        c = []
        for row in content:
            c.append(float(row[0]))
        return c

def readScatter(name):
    with open(name, newline='') as csvfile:
        content = csv.reader(csvfile, delimiter=';')
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
        C.append(math.sqrt(pow(i-j, 2)))
    print("[*] Erro médio: {}".format(sum(C)/len(C)))
    print("[*] Erro máximo: {}".format(max(C)))

def showData(fileName):
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    X=[] 
    Y=[] 
    Z=[]
    Zp=[]
    os.chdir("./")
    for file in glob.glob(fileName):
        name=file.split(".")[0]
        xs, ys, zs = readScatter("./"+name+".csv")
        c = readFile("./"+name+".dat")
        X = X+xs
        Y = Y+ys
        Z = Z+zs
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
        for w in range(0, len(xs)):
            s=0
            for i in range(0, len(c)):
                s += c[i] * A[w][i]
            Zp.append(s)
    ax.scatter(X, Y, Z)
    ax.scatter(X, Y, Zp)
    erro(Z, Zp)
    # Customize the z axis.
    ax.set_zlim(0.01, 1.01)
    ax.zaxis.set_major_locator(LinearLocator(10))
    ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))
    plt.draw()

def compareData(fileName):
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    c = readFile("./"+fileName+".dat")
    # Make data.
    X = np.arange(0, 1, 0.05)
    Y = np.arange(0, 1, 0.05)
    X, Y = np.meshgrid(X, Y)
    if(len(c) == 4):
        Zp = c[0] + c[1]*X + c[2]*Y + c[3]*X*Y
    elif(len(c) == 6):
        Zp = c[0] + c[1]*X + c[2]*Y + c[3]*X*Y + c[4]*(X**2)*Y + c[5]*(X**2)*(Y**2)
    else:
        Zp = c[0] + c[1]*X + c[2]*Y + c[3]*X*Y + c[4]*(X**2)*Y + c[5]*(X**2)*(Y**2) + c[6]*(X**3)*(Y**2) + c[7]*(X**3)*(Y**3)
    Z = f(X, Y)
    erro(Z, Zp)
    ax.plot_surface(X, Y, Zp)
    ax.plot_surface(X, Y, Z)
    # Customize the z axis.
    ax.set_zlim(0.01, 1.01)
    ax.zaxis.set_major_locator(LinearLocator(10))
    ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))
    plt.draw()

if __name__ == '__main__':
    print("[*] Local")
    fileName="binormal_*.csv"
    showData(fileName)
    print("[*] Global")
    fileName="binormal.csv"
    showData(fileName)
    plt.show()
