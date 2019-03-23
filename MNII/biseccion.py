from termcolor import colored
import numpy as np


def f(x):
    return np.log(x**2+1)-np.exp(x/2)*np.cos(3*x)


def calcularN(a, b, epsilon):
    return np.ceil(np.log((b-a)/epsilon)/np.log(2)-1)


def biseccion(f, a, b, epsilon, delta=10**(-15)):
    n = calcularN(a, b, epsilon)
    print(" - Número de pasos necesarios => ", colored("N="+str(n), "yellow"))
    print(colored("============= Iteraciones =============", "green"))
    print("n \t x_n \t f(x_n)")
    print()
    c_ant = 0
    c = 0
    for i in range(1, int(n)+1):
        c = (a+b)/2.0
        print(str(i) + "\t" + str(c) + "\t" + str(f(c)))
        if np.absolute(c_ant - c) < delta:
            return c
        else:
            aux = f(a)*f(c)
            c_ant = c
            if aux < 0:
                b = c
            else:
                a = c

    return c


def main():
    print(colored("========= Método de bisección =========", "red"))
    solBiseccion = biseccion(f, -1, 0, 10**(-6), 10**(-6))
    print(colored("Solución método bisección: ", "red"),
          colored(str(solBiseccion), "blue"))


main()
