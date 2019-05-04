from termcolor import colored
import numpy as np
from sympy import *
from math import *

x = Symbol('x')

# =====================================================================


def f(x):
    return np.exp(np.sin(x)**2-2)
    # return np.power(cos(pi * x)-1, 10/3.0)
    # return (cos(pi*x)+1) ** (10/3.0)


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


# -0.019349976075697217
# -2.314466535269233e-05
# -9.00015630911355e-08
# -4.192397540236925e-10
#  -2.035260848742837e-12
# -1.0658141036401503e-14


# 0.6539307238542182
# -0.036748306954090815
# 0.00015680136484608198
# 5.591016795314374e-06
# -2.400999132845527e-08
# 2.2005952615700153e-11
# sol = trapecioCompuesta(f, 0, 1, 64)
# print(colored("Solución método Trapecio Compuesto: ", "red"),
#       colored(str(sol), "blue"))


# print(3.000492123714049-sol)
# print(colored("Error método Trapecio Compuesto: ", "red"),
#       colored(str(errorT(0, 1, 8, 0.73576)), "blue"))

sol = simpsonCompuesta(f, 0, 2, 107)
print(colored("Solución método Simpson Compuesto: ", "red"),
      colored(str(sol), "blue"))
# print(colored("Error método simpson Compuesto: ", "red"),
#       colored(str(errorS(-4, 4, 10, 24)), "blue"))
