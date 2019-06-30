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

    print("Solucion: ", v)
