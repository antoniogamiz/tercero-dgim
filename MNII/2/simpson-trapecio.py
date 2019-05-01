from termcolor import colored
import numpy as np
from sympy import *
from math import factorial

x = Symbol('x')

# =====================================================================


def f(x):
    return (1/(1+x**2))

# =====================================================================


def nodos(a, b, n):
    nodes = []
    h = (b-a)/n
    for j in range(0, n+1):
        nodes.append(a+j*h)
    return nodes

# =====================================================================


def errorT(a, b, n, max):
    h = (b-a)/n
    return abs((((b-a)*h**2)/12)*max)


def trapecioCompuesta(f, a, b, n):
    result = 0
    nodes = nodos(a, b, n)
    result += (f(nodes[0]) + f(nodes[-1])) * 0.5
    for i in range(1, n):
        result = result + f(nodes[i])
    return result * (b-a)/n

# =====================================================================


def errorS(a, b, n, max):
    h = (b-a)/n
    return abs((((b-a)*h**4)/180)*max)


def simpsonCompuesta(f, a, b, m):
    result = 0
    nodes = nodos(a, b, 2*m)
    result += f(nodes[0]) + f(nodes[-1])
    for i in range(1, m+1):
        result = result + 4*f(nodes[2*i-1])
    for i in range(2, m+1):
        result = result + 2*f(nodes[2*i-2])
    return result * ((b-a)/(2*m))/3

# =====================================================================


sol = trapecioCompuesta(f, -4, 4, 10)
print(colored("Solución método Trapecio Compuesto: ", "red"),
      colored(str(sol), "blue"))
print(colored("Error método Trapecio Compuesto: ", "red"),
      colored(str(errorT(-4, 4, 10, 0.5)), "blue"))

sol = simpsonCompuesta(f, -4, 4, 10)
print(colored("Solución método Simpson Compuesto: ", "red"),
      colored(str(sol), "blue"))
print(colored("Error método simpson Compuesto: ", "red"),
      colored(str(errorS(-4, 4, 10, 24)), "blue"))
