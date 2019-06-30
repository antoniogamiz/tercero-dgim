from termcolor import colored
import numpy as np


def heun(f, a, b, n, y0):
    h = (b-a)/n
    uj = [y0]
    tj = 0
    for j in range(1, n+1):
        tj = a + (j-1)*h
        uj.append(uj[j-1] + h * (0.5*f(tj, uj[j-1]) +
                                 0.5*f(tj+h, uj[j-1]+h*f(tj, uj[j-1]))))
    return uj


def f(t, y):
    return -y+t+1


result = heun(f, 0, 1, 10, 1)

for x in result:
    print(x)
