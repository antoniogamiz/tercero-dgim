from termcolor import colored
import numpy as np
from sympy import *
from math import factorial

x = Symbol('x')

# =====================================================================


def f(x):
    return (1/(1+x**2))
    # return x

# =====================================================================


def error(a, b, n, max):
    h = (b-a)/n
    return abs((((b-a)*h**2)/12)*max)


def romberg(f, a, b, n):
    RKJ = [[]]
    # aniadimos el R00
    RKJ[0].append(((b-a)/2)*(f(a)+f(b)))
    for k in range(1, n+1):
        rk0 = 0
        hk = (b-a)/(2.0**k)
        for i in range(1, 2**(k-1)+1):
            rk0 += f(a+(2*i-1)*hk)
        rk0 *= hk
        rk0 += RKJ[0][k-1]*0.5
        RKJ[0].append(rk0)

    # ya tenemos los RK0, calculamos los otros
    for j in range(1, n+1):
        rkj = 0
        RKJ.append([])
        for k in range(0, n-j+1):
            # print(str(j) + " " + str(k))
            rkj = RKJ[j-1][k+1]+(1/(4**j-1))*(RKJ[j-1][k+1]+RKJ[j-1][k])
            RKJ[j].append(rkj)

    for l in RKJ:
        for l2 in l:
            print(str(l2) + " ", end='')
        print("\n")
    return RKJ[-1][-1]
# =====================================================================


sol = romberg(f, -4, 4, 4)
print(colored("Solución método Trapecio Compuesto: ", "red"),
      colored(str(sol), "blue"))
print(colored("Error método Trapecio Compuesto: ", "red"),
      colored(str(error(-4, 4, 10, 0.5)), "blue"))
