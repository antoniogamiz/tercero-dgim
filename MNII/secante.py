from termcolor import colored
import numpy as np


def f(x):
    return np.exp(x)-np.cos(x)


def secante(f, a, b, epsilon, delta=10**(-15)):
    print(colored("============= Iteraciones =============", "green"))
    print("n \t x_n \t f(x_n)")
    print()
    c = (b-(b-a)*f(b))/(f(b)-f(a))
    i = 0
    while np.abs(f(b)-f(a)) >= delta:
        c = b-(b-a)*f(b)/(f(b)-f(a))
        print(str(i) + "\t" + str(c) + "\t" + str(f(c)))

        # corremos un elemento
        a = b
        b = c

        i = i + 1
    return c


print(colored("========= Método de la secante =========", "red"))
solSecante = secante(f, -2, -1, 10**(-5))
print(colored("Solución método secante: ", "red"),
      colored(str(solSecante), "blue"))
