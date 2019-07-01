import numpy as np


def f(x):
    return (5*x**2+7)/(x**2+3)


def steffsen(f, x_0, delta=10**(-15)):
    # valores iniciales
    x = []
    x.append(x_0)
    x_1 = f(x_0)
    x_2 = f(x_1)
    print("============= Iteraciones =============")
    print("n \t x_n")
    print("0" + "\t" + str(x[0]))
    i = 1
    cont = True
    while cont:
        newX = x[i-1] - ((x_1-x[i-1])**2)/(x_2-2*x_1+x[i-1])
        x.append(newX)
        x_1 = f(newX)
        x_2 = f(x_1)
        print(str(i) + "\t" + str(x[i]))
        cont = np.abs(x[i]-x[i-1]) > delta
        i = i + 1

    return x[len(x) - 1]


steffsen(f, 5)
