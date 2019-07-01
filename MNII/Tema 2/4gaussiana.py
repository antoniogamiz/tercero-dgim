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


def gaussiana(f, a, b, n, nodos):
    result = 0
    for i in range(0, n+1):
        result = result + integrate(l(nodos, i),
                                    (x, -1, 1)) * f((nodos[i]*(b-a)+(b-a))/2)
    return result * (b-a)/2

# =====================================================================

# AQUI EL MAXIMO DE LA DERIVADA TAMBIEN LO CALCULAMOS REPRESNETANDO LA DERIVADA Y VIENDOLO
# A OJILLO


# el ultimo array que se la pasa son las raices del polinomio de legendre!!
sol = gaussiana(f, 0, 2, 3, [-0.861136, -0.339981, 0.339981, 0.861136])
print("Solución método Gaussiana: ",
      str(sol))
