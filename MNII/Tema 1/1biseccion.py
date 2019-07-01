import numpy as np


# este metodo calcula el numero de iteraciones necesarias para
# conseguir el error tan pequeño como queremos (el error que queremos
# es epsilon)
def calcularN(a, b, epsilon):
    return np.ceil(np.log((b-a)/epsilon)/np.log(2)-1)

# delta se usa para la condicion de parada pero puedes ignorarlo


def biseccion(f, a, b, epsilon, delta=10**(-15)):
    n = calcularN(a, b, epsilon)
    print(" - Número de pasos necesarios => N= "+str(n))
    print("============= Iteraciones =============")
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


def f(x):
    return np.log(x**2+1)-np.exp(x/2)*np.cos(3*x)


print("========= Método de bisección =========")
solBiseccion = biseccion(f, -1, 0, 10**(-6), 10**(-6))
print("Solución método bisección: ",
      str(solBiseccion))
