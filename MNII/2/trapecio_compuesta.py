import sympy as sp
import math as m

x = sp.symbols('x')
sp.init_printing(use_unicode=True)

#f=sp.Lambda(x, x*sp.cos(x)-x**2*sp.sin(x))


def f(x):
    return (m.cos(m.pi * x) - 1) ** (10/3.0)
    # return x*m.cos(x)-x**2*m.sin(x)


def trapecio_compuesta(f, a, b, n):

    h = (b-a)/n

    result = (f(a)+f(b))/2

    for i in range(1, n):
        result += f(a+i*h)

    result *= h

    return result


a, b = 0, 1
n = 64

result = trapecio_compuesta(f, a, b, n)

print(result)

# Error

h = (b-a)/n

cota_2 = 200

# print("Cota error: "+str(abs((b-a)*h**2/12*cota_2)))
