int k;
int bf[k];
int contador = 0;


Semaphore prodcucir(k), consumir(0);


escribir(dato){
    sem_wait( poducir );
    bf[contador++]=dato;
    sem_signal( consumir );
}

lee(dato){
    sem_wait( consumir );
    contador--;
    dato=bf[contador];
    sem_signal( producir );
    consumir(dato);
}


Con la estructura fifo podemos quitar el semaforo de exclusion mutua porque solo hay 1 consumidor, pero si hay MAS DE UN PRODUCTOR, entonces hay que ponerlo sobre los dos consumidores.