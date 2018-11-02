ejercicio: Implementar un semáforo a través de un monitor con semántica SU.

monitor Semaphore:
    // variables permanentes
    int value;

    // procedures
    procedure wait () {
        if( value == 0 )
            cola.wait()
        value--
    }

    procedure signal () {
        value++
        cola.signal()º
    }