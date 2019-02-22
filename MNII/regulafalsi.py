from termcolor import colored
import numpy as np


def f(x):
    return np.exp(x)-np.cos(x)


def regulafalsi(f, a, b, epsilon, delta=10**(-15)):
    print(colored("============= Iteraciones =============", "green"))
    print("n \t x_n \t f(x_n)")
    print()
    c = (b-(b-a)*f(b))/(f(b)-f(a))
    i = 0
    while np.abs(f(c)) >= delta:
        c = b-(b-a)*f(b)/(f(b)-f(a))
        print(str(i) + "\t" + str(c) + "\t" + str(f(c)))
        aux = f(a)*f(c)
        if aux < 0:
            b = c
        else:
            a = c
        i = i + 1
    return c


print(colored("========= Método de regula falsi =========", "red"))
solFalsi = regulafalsi(f, -2, -1, 10**(-5))
print(colored("Solución método regula falsi: ", "red"),
      colored(str(solFalsi), "blue"))
