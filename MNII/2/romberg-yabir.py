import sympy as sp
from sympy.abc import x


def trapecio_compuesta(f, a, b, n):

    h = (b-a)/n

    result = (f(a)+f(b))/2

    for i in range(1, n):
        result += f(a+i*h)

    result *= h

    return result


def romberg(a, b, n, f):

    R = [[0 for _ in range(n+1)] for _ in range(n+1)]

    R[0][0] = trapecio_compuesta(f, a, b, 1)

    for k in range(1, n+1):

        hk = (b-a)/2**k
        aux = 0
        for i in range(1, 2**(k-1)+1):
            aux += f(a+(2*i-1)*hk)
        R[k][0] = R[k-1][0]/2+hk*aux

    for j in range(1, n+1):
        for k in range(j, n+1):
            R[k][j] = R[k][j-1]*(1+1/(4**j-1))-1/(4**j-1)*R[k-1][j-1]

    return R[n][n], R


if __name__ == '__main__':

    import math

    def f(x):
        return (math.cos(math.pi*x)+1) ** (10/3.0)

    n = 7
    v, R = romberg(0, 1, n, f)

    I = sp.integrate((sp.cos(x)+1)**(10/3.0), (x, 0, 1))

    dR = [[0 for i in range(n+1)] for j in range(n+1)]

    # for i in range(len(R)):
    #     for j in range(i+1):
    #         dR[i][j] = abs(R[i][j]-I)

    # for row in dR:
    #     print(row)

    print(v)
    print(3.000492123714049-v)

# 0.6539307238542182
# -0.036748306954090815
# 0.00015680136484608198
# 5.591016795314374e-06
# -2.400999132845527e-08
# 2.2005952615700153e-11
"""
22 
5 
1.2656
0.31 1
0.077
0.019
0.004
0.001

El error de la formula del trapecio compuesta es 

(b-a)/12 * (b-a)/2**k * f''(eta) 

como cambia el error¿? la derivada segunda no tiene por qué ser la
misma para cada intervalo, cambia según la subdivision. SI suponemos
que es parecido el error de una a la siguiente se divide por
cuatro. Aproximandamente el error es un cuarto del anterior.

La primera tiene en la diagonal 0.47 0.059 7,4*10**-5   --- 11.5*10**-14

Para la segunda función

0.1666
0.063
0.0085
0.0035
0.0011
3.9 * 10**(-4)
1.4 * 10**(-4)

La diagonal es 

0.02859
0.0089
0.0030
0.00107
3.7896*10**-4

no es lo mismo por las derivadas, La función no es continua en el 0
con lo que el desarrollo no mejora. Todas las columnas converjen pero
el orden de convergencia apenas mejora.


En principio la diferencia del error debe ser también aproximadamente
dividido por 4

¿Qué es lo que cambia respecto a la otra?¿Por qué no es tan
exactamente un 4?
¿Como evolucionan los errores? Eliminamos un termino en el desarrollo

en el trapecio compuesto era c1h**2 + c2h**4...

Cuando pasamos de una columna tenemos h_{-1}**2



"""
