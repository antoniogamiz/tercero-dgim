.SUFFIXES:
.PHONY:    pp,clean

compilador := mpicxx -std=c++11
flagsc     := -g -Wall

pp: primes_pipeline_exe
	mpirun -np  10 ./$<

ti1: transformacion_iterativa_1_exe
	mpirun -np  4 ./$<

ti2: transformacion_iterativa_2_exe
	mpirun -np  4 ./$<

%_exe: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -rf *_exe *.dSYM
