# Assingment2

Como usar:

    gcc -o output assignment.c auxiliar.c -lm

    ./output <arquivo de pontos da binormal> <grau do polinomio>

Resultado:
    arquivo "results.dat" com os valores dos coeficientes dos polinomios

Exemplo [5 pontos, grau 2]
    
    arquivo binormal.txt:
        
        -0.9441;1.7682;1.0166
        -0.6741;-1.3614;-0.1728
        0.56;-0.8731;1.6253
        -0.7896;0.1114;0.0363
        0.2153;1.065;-0.8465

    comando:
    
        ./output binormal.txt 2

    resultado no terminal:

                                          [A]
|+1.00000|       |-0.94410|      |+1.76820|      |-1.66936|      |+1.57604|      |+2.78675|      
|+1.00000|       |-0.67410|      |-1.36140|      |+0.91772|      |-0.61863|      |+0.84221|      
|+1.00000|       |+0.56000|      |-0.87310|      |-0.48894|      |-0.27380|      |+0.23906|      
|+1.00000|       |-0.78960|      |+0.11140|      |-0.08796|      |+0.06945|      |+0.00774|      
|+1.00000|       |+0.21530|      |+1.06500|      |+0.22929|      |+0.04937|      |+0.05258|      

b[0] = 1.01660
b[1] = -0.17280
b[2] = 1.62530
b[3] = 0.03630
b[4] = -0.84650

                                   [At]
|+1.00000|       |+1.00000|      |+1.00000|      |+1.00000|      |+1.00000|      
|-0.94410|       |-0.67410|      |+0.56000|      |-0.78960|      |+0.21530|      
|+1.76820|       |-1.36140|      |-0.87310|      |+0.11140|      |+1.06500|      
|-1.66936|       |+0.91772|      |-0.48894|      |-0.08796|      |+0.22929|      
|+1.57604|       |-0.61863|      |-0.27380|      |+0.06945|      |+0.04937|      
|+2.78675|       |+0.84221|      |+0.23906|      |+0.00774|      |+0.05258|      

                                          [M]
|+5.00000|       |-1.63250|      |+0.71010|      |-1.09924|      |+0.80242|      |+3.92834|      
|-1.63250|       |+2.32916|      |-1.09924|      |+0.80242|      |-1.26846|      |-3.05963|      
|+0.71010|       |-1.09924|      |+6.88888|      |-3.53985|      |+3.92834|      |+3.62909|      
|-1.09924|       |+0.80242|      |-3.53985|      |+3.92834|      |-3.05963|      |-3.98469|      
|+0.80242|       |-1.26846|      |+3.92834|      |-3.05963|      |+2.94884|      |+3.80870|      
|+3.92834|       |-3.05963|      |+3.62909|      |-3.98469|      |+3.80870|      |+8.53529|      

bp[0] = 1.65890
bp[1] = -0.14403
bp[2] = -0.28373
bp[3] = -2.84761
bp[4] = 1.22482
bp[5] = 3.03180
[*] Cholesky

                                          [L]
|+2.23607|       |+0.00000|      |+0.00000|      |+0.00000|      |+0.00000|      |+0.00000|      
|-0.27394|       |+1.50137|      |+0.00000|      |+0.00000|      |+0.00000|      |+0.00000|      
|+0.62979|       |-0.71873|      |+2.44452|      |+0.00000|      |+0.00000|      |+0.00000|      
|-0.40684|       |+0.96394|      |-0.15790|      |+1.67592|      |+0.00000|      |+0.00000|      
|+0.55733|       |-0.59694|      |+0.12994|      |-0.26674|      |+1.48117|      |+0.00000|      
|+0.11384|       |-0.21993|      |+0.12034|      |-0.16095|      |+0.18989|      |+2.89785|      

y[0] = 0.24540
y[1] = 0.60693
y[2] = -0.20228
y[3] = -0.87942
y[4] = 0.16630
y[5] = 0.31922

                                          [Lt]
|+2.23607|       |-0.27394|      |+0.62979|      |-0.40684|      |+0.55733|      |+0.11384|      
|+0.00000|       |+1.50137|      |-0.71873|      |+0.96394|      |-0.59694|      |-0.21993|      
|+0.00000|       |+0.00000|      |+2.44452|      |-0.15790|      |+0.12994|      |+0.12034|      
|+0.00000|       |+0.00000|      |+0.00000|      |+1.67592|      |-0.26674|      |-0.16095|      
|+0.00000|       |+0.00000|      |+0.00000|      |+0.00000|      |+1.48117|      |+0.18989|      
|+0.00000|       |+0.00000|      |+0.00000|      |+0.00000|      |+0.00000|      |+2.89785|      

c[0] = 0.04908
c[1] = 0.26926
c[2] = -0.03385
c[3] = -0.31310
c[4] = 0.07580
c[5] = 0.03801

    arquivo de resultado "results.dat":
        0.049081
        0.269256
        -0.033851
        -0.313105
        0.075803
        0.038014


    plot do resultado:
        <TODO>
