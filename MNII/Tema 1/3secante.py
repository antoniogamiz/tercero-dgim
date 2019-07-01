import numpy as np


def secante(f, a, b, epsilon, delta=10**(-15)):
    print("============= Iteraciones =============")
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


def f(x):
    return np.exp(x)-np.cos(x)


print("========= Método de la secante =========")
solSecante = secante(f, -2, -1, 10**(-5))
print("Solución método secante: ",
      str(solSecante))
