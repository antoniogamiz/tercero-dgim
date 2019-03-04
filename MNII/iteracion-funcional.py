from termcolor import colored
import numpy as np


def f(x):
    return (5*x**2+7)/(x**2+3)


def iFuncional(f, x_0, delta=10**(-15)):
    x_n = []
    x_n.append(x_0)
    print(colored("============= Iteraciones =============", "green"))
    print("n \t x_n \t f(x_n)")
    print()
    i = 0
    print(str(i) + "\t" + str(x_n[i]))
    x_n.append(f(x_n[0]))
    i = 1
    while np.abs(f(x_n[i])-f(x_n[i-1])) > delta:
        print(str(i) + "\t" + str(x_n[i]))
        i = i + 1
        x_n.append(f(x_n[i-1]))

    return x_n[len(x_n) - 1]


def main():
    iFuncional(f, 5)


# main()
