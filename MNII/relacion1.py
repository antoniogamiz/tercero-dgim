from biseccion import biseccion
from regulafalsi import regulafalsi


def g(x):
    return x**3+4*x**2-10


def g2(x):
    return x**3-56

# Ejercicio 1
# biseccion(f, 1, 2, 10**(-5))
# print("Numero de iteraciones necesarias para un error inferior a 10**-8")
# print(calcularN(1, 2, 10**(-8)))


# Ejercicio 2
# biseccion(g2, 3, 4, 10**(-8))
regulafalsi(g2, 3, 4, 10**(-8), 10**(-14))
