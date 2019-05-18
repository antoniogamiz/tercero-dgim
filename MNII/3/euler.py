from termcolor import colored
import numpy as np


def euler(f, a, b, n, y0):
    h = (b-a)/n
    uj = [y0]
    tj = 0
    for j in range(1, n+1):
        tj = a + (j-1)*h
        uj.append(uj[j-1]+h*f(tj, uj[j-1]))
    return uj


def main():
    # ejemplo diapositiva 15
    def f(t, y):
        return -y+t+1

    result = euler(f, 0, 1, 10, 1)

    for x in result:
        print(x)


main()
