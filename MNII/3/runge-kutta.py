from termcolor import colored
import numpy as np
from columnar import columnar


def runge(f, a, b, n, y0):
    h = (b-a)/n
    uj = [y0]
    tj = 0
    result = []
    y = 0
    for j in range(1, n+1):
        tj = a + (j-1)*h
        K1 = f(tj, uj[j-1])
        K2 = f(tj + h/2, uj[j-1] + (h/2)*K1)
        K3 = f(tj + h/2, uj[j-1] + (h/2)*K2)
        K4 = f(tj + h, uj[j-1] + h*K3)

        uj.append(uj[j-1] + (h/6) * (K1 + 2*K2 + 2*K3 + K4))
        y = (2*tj+1)/(tj**2+1)
        result.append([y, uj[j], y-uj[j]])
    return result


def main():
    # ejemplo diapositiva 15
    def f(t, y):
        return -y+t+1

    result = runge(f, 0, 1, 10, 1)

    for x in result:
        print(x)


main()
