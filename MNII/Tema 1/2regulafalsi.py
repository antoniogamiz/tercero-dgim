import numpy as np


def regulafalsi(f, a, b, epsilon, delta=10**(-15)):
    print("============= Iteraciones =============")
    print("n \t x_n \t f(x_n)")
    print()
    c = (b-(b-a)*f(b))/(f(b)-f(a))
    i = 0
    while np.abs(f(c)) > delta:
        c = b-(b-a)*f(b)/(f(b)-f(a))
        print(str(i) + "\t" + str(c) + "\t" + str(f(c)))
        aux = f(a)*f(c)
        if aux < 0:
            b = c
        else:
            a = c
        i = i + 1
    return c


def f(x):
    return np.log(x**2+1)-np.exp(x/2)*np.cos(3*x)


print("========= Método de regula falsi =========")
solFalsi = regulafalsi(f, -1, 0, 10**(-5), 10**(-6))
print("Solución método regula falsi: ",
      str(solFalsi))
