from termcolor import colored
import numpy as np
from sympy import *


def newtonraphson(f, a, b, epsilon, delta=10**(-15)):
    print(colored("============= Iteraciones =============", "green"))
    print("n \t x_n \t f(x_n)")
    print()

    # tomamos el punto medio del intervalo como aproximacion inical
    x_0 = 0.4
    x_1 = x_0 - f(x_0)/f_diff(x_0)
    print(str(0) + "\t" + str(x_0) + "\t" + str(f(x_0)))

    i = 1
    while i <= 10:
        # corremos un elemento
        x_0 = x_1
        x_1 = x_0 - f(x_0)/f_diff(x_0)

        print(str(i) + "\t" + str(x_0) + "\t" + str(f(x_0)))
        i = i + 1

    return x_1


def f(x):
    return log(x**2+1)-exp(x/2)*cos(3*x)


def f_diff(x_0):
    x = Symbol('x')
    # DONDE PONE y = (...) TAMBIEN TIENES QUE ESCRIBIR f!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    # CON MODIFICAR ESTA LINEA Y LA DEFINICION DE LA f YATA
    y = log(x**2+1)-exp(x/2)*cos(3*x)
    fprime = diff(y, x)
    return fprime.evalf(subs={x: x_0})


print(colored("========= Método de Newton-Raphson =========", "red"))
solNewton = newtonraphson(f, 0, 1, 10**(-5), 10**(-6))
print(colored("Solución método Newton-Raphson: ", "red"),
      colored(str(solNewton), "blue"))
