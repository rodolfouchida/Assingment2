#!/usr/bin/env python3
import numpy as np
import os, glob, math, csv
import sys, platform, subprocess
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

from math import pi, sqrt, exp

# gerar dois arquivos
#  Gerar com distribuição uniforme de x, y
#  1. Pontos x, y, z para calcular os coeficientes dos polinomios
#  2. Pontos x, y para plotar os polinomios com coeficientes calculados no passo 1, calcular z
#  3. Plot de superficie. Na hora de obter os pontos pode ser 1000 para a região inteira, mas para plot pegar 100 (valor fixo) por quadrado no grid

# Propriedades da distribuição binormal
#   x = sigma1, mu1
#   y = sigma2, mu2
#   rho = ?
sigma1 = 0.5
sigma2 = 0.5
mu1 = 0.5
mu2 = 0.5
rho = 0
# Prefixo para arquivos de saida
prefix="binormal"
# Codigo C compilado
srccode="./src/output"

def f(x,y):
    tmp1 = 2*pi*sigma1*sigma2*sqrt(1-pow(rho,2))
    tmp2 = exp(-Q(x,y)/2)
    return (1/tmp1)*tmp2

def Q(x,y):
    tmp1 = x-mu1
    tmp2 = y-mu2
    tmp3 = 1-pow(rho,2)
    return (1/tmp3)*( pow(tmp1/sigma1,2) - 2*rho*(tmp1/sigma1)*(tmp2/sigma2) + pow(tmp2/sigma2, 2) )

def chunkIt(seq, num):
    avg = len(seq) / float(num)
    out = []
    last = 0.0

    while last < len(seq):
        out.append(seq[int(last):int(last + avg)])
        last += avg

    return out

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

def generate(name='binormal', npontos=1000, nbins=5, grau="3"):
    x =  np.random.uniform(0,1,npontos)
    y =  np.random.uniform(0,1,npontos)
    z = []
    for i,j in zip(x,y):
        z.append(f(i,j))

    P1 = []
    for i, j, k in zip(x, y, z):
        P1.append((i, j, k))
    P = sorted(P1, key=lambda element:(element[0], element[1]))

    fileName = name
    with open(fileName+".csv", "w+") as doc:
        for i in P:
            doc.write("{};{};{}\n".format(i[0],i[1],i[2]))
    doc.closed
    subprocess.call(srccode+" "+fileName+" "+grau, shell=True)


    P2 = chunkIt(P, pow(nbins,2))
    #print(*P2,  sep = '\n')

    for k, i in enumerate(P2):
        fileName = name+"_"+str(k)
        with open(fileName+".csv", "w+") as doc:
            for j in i:
                doc.write("{};{};{}\n".format(j[0],j[1],j[2]))
        doc.closed
        subprocess.call(srccode+" "+fileName+" "+grau, shell=True)


if __name__=='__main__':
    subprocess.call("rm "+prefix+"*", shell=True)
    if len(sys.argv) == 4:
        generate(prefix, int(sys.argv[1]), int(sys.argv[2]), sys.argv[3])
    else:
        print("Usage: ./generateBinormal <#pontos> <#bins> <grau do polinomio>")

    print("[*] Local")
    fileName="binormal_*.csv"
    showData(fileName)
    print("[*] Global")
    fileName="binormal.csv"
    showData(fileName)
    plt.show()

