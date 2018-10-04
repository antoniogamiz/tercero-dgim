### Ejercicio propuesto:

Un productor 3 consumidores, buffer de tamaño 5. Cada dato producido por el productor será eliminado del buffer cuando sea leido por los 3 consumidores.

Zona Compartida
~~~
Semaphore puede_producir = 3;
Semaphore puede_consumir[3] = {0,0,0};
~~~



Productor
~~~
int dato;
Produce(dato)

wait(sem_producir);
wait(sem_producir);
wait(sem_producir);

x=dato

for i=0 to 2 signal(puede_consumir[i]);
~~~



Consumidor[i]
~~~
wait(puede_consumir[i]);
dato=x
signal(puede_producir);
Consume_dato(dato)
~~~