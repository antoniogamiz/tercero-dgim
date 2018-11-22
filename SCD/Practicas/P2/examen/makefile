.SUFFIXES:
.PHONY: x1, x2, clean

compilador:=g++
opcionesc:= -std=c++11 -pthread -I.
i:= HoareMonitor.cpp

all: fumadores barbero

fumadores: fumadores.cpp $(hmonsrcs)
	$(compilador) $(opcionesc) -o $@ $< HoareMonitor.cpp Semaphore.cpp

barbero: barbero.cpp $(hmonsrcs)
	$(compilador) $(opcionesc) -o $@ $< HoareMonitor.cpp Semaphore.cpp

clean:
	rm -f fumadores barbero