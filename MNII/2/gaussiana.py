from termcolor import colored
import numpy as np
from sympy import *
from math import factorial

x = Symbol('x')

# =====================================================================


def f(x):
    return (x**4) * exp(-2*(x**2))

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


def error(a, b, n, max):
    return 0
# =====================================================================


def gaussiana(f, a, b, n, nodos):
    result = 0
    for i in range(0, n+1):
        result = result + integrate(l(nodos, i),
                                    (x, -1, 1)) * f((nodos[i]*(b-a)+(b-a))/2)
    return result * (b-a)/2

# =====================================================================


sol = gaussiana(f, 0, 2, 3, [-0.861136, -0.339981, 0.339981, 0.861136])
print(colored("Solución método Newton-Cotes: ", "red"),
      colored(str(sol), "blue"))
print(colored("Error método Newton-Cotes: ", "red"),
      colored(str(error(0, 2, 6, 80640)), "blue"))
