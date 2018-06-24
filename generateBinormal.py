#!/usr/bin/env python3
import numpy as np
import os, argparse, sys, platform, math, subprocess
from math import pi, sqrt, exp
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

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

def generate(name='binormal.txt', npontos=100, nbins=5):

    x =  np.random.uniform(0,1,npontos)
    y =  np.random.uniform(0,1,npontos)
    z = []
    for i,j in zip(x,y):
        z.append(f(i,j))

    P1 = []
    for i, j, k in zip(x, y, z):
        P1.append((i, j, k))
    P = sorted(P1, key=lambda element:(element[0], element[1]))

    with open(name, "w+") as doc:
        for i in P:
            doc.write("{};{};{}\n".format(i[0],i[1],i[2]))
    doc.closed

    # Plot de pontos gerados
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.scatter(x, y, z)

    P2 = chunkIt(P, pow(nbins,2))
    #print(*P2,  sep = '\n')

    k=0
    for i in P2:
        with open("binormal_"+str(k)+".csv", "w+") as doc:
            for j in i:
                doc.write("{};{};{}\n".format(j[0],j[1],j[2]))
        doc.closed
        k += 1

    # Mostrar plots
    #plt.show()    

    ''' 
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
    with open(name, "w+") as doc:
        for i in xt:
            for j in yt:
                print("[*] {},{},{}\n".format(i,j,zt[k]))
                doc.write("{};{};{}\n".format(i,j,zt[k]))
                k += 1
    doc.closed
    plt.imshow(z, extent=extent, interpolation='nearest')
    plt.colorbar()
    plt.show()
    '''

if __name__=='__main__':
    #print("[*] Python version: {}".format(sys.version))
    #print("[*] Python version: {}".format(platform.python_version()))
    
    subprocess.call("rm binormal*", shell=True)
    if len(sys.argv) > 1:
        generate('binormal.txt', int(sys.argv[1]), int(sys.argv[2]))
    else:
        generate()
