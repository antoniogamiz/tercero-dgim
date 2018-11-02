.SUFFIXES:
.PHONY: x1, x2, clean

compilador:=g++
opcionesc:= -std=c++11 -pthread -Wfatal-errors
hmonsrcs:= HoareMonitor.h HoareMonitor.cpp Semaphore.h Semaphore.cpp


all: n_prodcons_sc_lifo_exe n_prodcons_su_fifo_exe n_prodcons_su_lifo_exe

n_prodcons_su_lifo_exe: n_prodcons_su_lifo.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

n_prodcons_su_fifo_exe: n_prodcons_su_fifo.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

n_prodcons_sc_lifo_exe: n_prodcons_sc_lifo.cpp
	$(compilador) $(opcionesc)  -o $@ $<

clean:
	rm -f *_exe
