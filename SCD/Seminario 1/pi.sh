#!/usr/bin/bash

# Script para generar los datos necesarios para hacer gráficas de ganancia en función
# del número de threads y el número de intervalos.

g++ -std=c++11 -pthread -o pi ejemplo09.cpp

for ((N=2;N<9;N=N*2))
do
    ./pi $N 1000
    ./pi $N 1000000
    ./pi $N 1000000000
    ./pi $N 2000000000
done