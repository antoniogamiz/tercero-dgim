from termcolor import colored
import numpy as np
from columnar import columnar


def runge(f, a, b, n, y0):
    h = (b-a)/n
    uj = [y0]
    tj = 0
    y = 0
    for j in range(1, n+1):
        tj = a + (j-1)*h
        K1 = f(tj, uj[j-1])
        K2 = f(tj + h/2, uj[j-1] + (h/2)*K1)
        K3 = f(tj + h/2, uj[j-1] + (h/2)*K2)
        K4 = f(tj + h, uj[j-1] + h*K3)

        uj.append(uj[j-1] + (h/6) * (K1 + 2*K2 + 2*K3 + K4))
    return uj


def euler(f, a, b, n, y0):
    h = (b-a)/n
    uj = [y0]
    tj = 0
    for j in range(1, n+1):
        tj = a + (j-1)*h
        uj.append(uj[j-1]+h*f(tj, uj[j-1]))
    return uj


def euler2(f, a, b, n, y0):
    h = (b-a)/n
    uj = [y0]
    tj = 0
    for j in range(1, n+1):
        tj = a + (j-1)*h
        uj.append(uj[j-1]+h*f(tj+h/2, uj[j-1]+(h/2)*f(tj, uj[j-1])))
    return uj


def fj(j, uj, a=0, h=0.1):
    # ejemplo diapositiva 15
    def f(t, y):
        return (2-2*t*y)/(1+t**2)
    return f(a+j*h, uj)


def adams(f, a, b, n, y0, aprox):
    h = (b-a)/n
    uj = aprox
    for j in range(3, n+1):
        tj = a + j*h
        uj.append(uj[j]+(h/24)*(55*fj(j, uj[j])-59*fj(j-1, uj[j-1]) +
                                37*fj(j-2, uj[j-2])-9*fj(j-3, uj[j-3])))

    return uj[n]


def main():
    # ejemplo diapositiva 15
    def f(t, y):
        return (2-2*t*y)/(1+t**2)

    def sol(t):
        return (2*t+1)/(t**2+1)

    aprox = euler(f, 0, 1, 3, 1)
    x = adams(f, 0, 1, 10, 1, aprox)
    print("Valor: exacto ", sol(1), "Aprox: ", x, " Error: ", np.abs(sol(1)-x))
    aprox = euler2(f, 0, 1, 3, 1)
    x = adams(f, 0, 1, 10, 1, aprox)
    print("Valor: exacto ", sol(1), "Aprox: ", x, " Error: ", np.abs(sol(1)-x))
    aprox = runge(f, 0, 1, 3, 1)
    x = adams(f, 0, 1, 10, 1, aprox)
    print("Valor: exacto ", sol(1), "Aprox: ", x, " Error: ", np.abs(sol(1)-x))


main()
