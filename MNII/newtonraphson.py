from termcolor import colored
import numpy as np
from sympy import *


def f(x):
    return exp(x)-cos(x)


def f_diff(x_0):
    x = Symbol('x')
    y = exp(x)-cos(x)
    fprime = diff(y, x)
    return fprime.evalf(subs={x: x_0})


def newtonraphson(f, a, b, epsilon, delta=10**(-15)):
    print(colored("============= Iteraciones =============", "green"))
    print("n \t x_n \t f(x_n)")
    print()

    # tomamos el punto medio del intervalo como aproximacion inical
    x_0 = (a+b)/2.0
    x_1 = x_0 - f(x_0)/f_diff(x_0)
    print(str(0) + "\t" + str(x_0) + "\t" + str(f(x_0)))

    i = 1
    while np.abs(f(x_1)-f(x_0)) > delta:
        # corremos un elemento
        x_0 = x_1
        x_1 = x_0 - f(x_0)/f_diff(x_0)

        i = i + 1
        print(str(i) + "\t" + str(x_0) + "\t" + str(f(x_0)))

    return x_1


print(colored("========= Método de Newton-Raphson =========", "red"))
solNewton = newtonraphson(f, -2, -1, 10**(-5))
print(colored("Solución método Newton-Raphson: ", "red"),
      colored(str(solNewton), "blue"))
