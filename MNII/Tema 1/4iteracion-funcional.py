from termcolor import colored
import numpy as np


def iFuncional(f, x_0, n, delta=10**(-15)):
    x_n = []
    x_n.append(x_0)
    print(colored("============= Iteraciones =============", "green"))
    print("n \t x_n \t f(x_n)")
    print()
    i = 0
    print(str(i) + "\t" + str(x_n[i]))
    x_n.append(f(x_n[0]))
    i = 1
    while i <= n:
        print(str(i) + "\t" + str(x_n[i]))
        i = i + 1
        x_n.append(f(x_n[i-1]))

    return x_n[len(x_n) - 1]


def f(x):
    return -1*(np.log(x**2+1)-np.exp(x/2)*np.cos(3*x))/3.0+x


# funciion f, condicion inicial, numero de iteraciones
iFuncional(f, 0.4, 10)
