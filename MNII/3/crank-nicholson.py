from termcolor import colored
from sympy import *

y = Symbol('y')


def crankNicholson(f, a, b, n, y0):
    h = (b-a)/n
    uj = [y0]
    tj = 0
    for j in range(1, n+1):
        tj = a + (j-1)*h
        solution = solve(y-uj[j-1]-(h/2)*(f(tj, uj[j-1])+f(a+j*h, y)))[0]
        uj.append(solution)
    return uj


def main():
    # ejemplo diapositiva 15
    def f(t, y):
        return -y+t+1

    result = crankNicholson(f, 0, 1, 10, 1)

    for x in result:
        print(x)


main()
