from termcolor import colored
import numpy as np
from sympy import *
from math import factorial

x = Symbol('x')

# =====================================================================


def f(x):
    return np.exp(np.sin(x)**2-2)

# =====================================================================


def nodos(a, b, n):
    nodes = []
    h = (b-a)/n
    for j in range(0, n+1):
        nodes.append(a+j*h)
    return nodes

# =====================================================================


def l(nodes, i):
    li = 1
    for j in range(0, len(nodes)):
        if j != i:
            li = li * (x-nodes[j])/(nodes[i]-nodes[j])
    return li

# =====================================================================

# esta funcion da el error para las aproximaciones de newton cotes abiertas
# y cerradas!! ella sola sabe si es abierta o cerrada


def error(a, b, n, max):
    y = Symbol('y')
    h = (b-a)/n
    kn = 1
    if n % 2 == 0:
        kn = y**2
        for i in range(1, n+1):
            kn = kn * (y-i)
        kn = integrate(kn, (y, 0, n)) / factorial(n+2)
        return abs(kn * (h**(n+3))*max)
    else:
        kn = y
        for i in range(1, n+1):
            kn = kn * (y-i)
        kn = integrate(kn, (y, 0, n)) / factorial(n+1)
        return abs(kn * (h**(n+2))*max)

# =====================================================================


def newtonCotes(f, a, b, n):
    result = 0
    nodes = nodos(a, b, n)
    for i in range(0, n+1):
        result = result + integrate(l(nodes, i), (x, a, b)) * f(nodes[i])
    return result

# =====================================================================


# se le pasa la funcion f, el intervalo a,b y el numero de iteraciones
sol = newtonCotes(f, 0, 2, 9)
print(colored("Solución método Newton-Cotes: ", "red"),
      colored(str(sol), "blue"))
print(colored("Error método Newton-Cotes: ", "red"),
      colored(str(error(0, 2, 6, 80640)), "blue"))
