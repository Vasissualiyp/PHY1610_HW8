# makefile for the wave2d application

CXX=g++

DBGFLAGS=-g
OPTFLAGS= -O2 -march=native
OPENMPFLAG= -fopenmp
GPROFFLAGS= -pg -gstabs
CXXFLAGS=$(OPTFLAGS) $(DBGFLAGS) $(GPROFFLAGS) -std=c++17 -Wall -Wfatal-errors -Wconversion -Wno-narrowing -Wno-sign-compare -isystem ${MODULE_NETCDF_PREFIX}/include
LDLIBS=-lnetcdf -lnetcdf_c++4
LDFLAGS=$(OPTFLAGS) $(GPROFFLAGS) $(OPENMPFLAG) $(DBGFLAGS) 

all: wave2d

wave2d: wave2d.o parameters.o initialize.o output.o evolve.o simulation.o timing_output.o 
	$(CXX) $(LDFLAGS) -o $@ $^  $(LDLIBS)

wave2d.o: wave2d.cpp parameters.h wavetypes.h initialize.h output.h evolve.h simulation.h
	$(CXX) -c $(CXXFLAGS) -o wave2d.o wave2d.cpp $(OPENMPFLAG)

parameters.o: parameters.cpp parameters.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o parameters.o parameters.cpp $(OPENMPFLAG)

initialize.o: initialize.cpp initialize.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o initialize.o initialize.cpp $(OPENMPFLAG)

output.o: output.cpp output.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o output.o output.cpp $(OPENMPFLAG)

evolve.o: evolve.cpp evolve.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o evolve.o evolve.cpp $(OPENMPFLAG)

simulation.o: simulation.cpp simulation.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o simulation.o simulation.cpp $(OPENMPFLAG)

timing_output.o: timing_output.cpp 
	$(CXX) -c $(CXXFLAGS) -o timing_output.o timing_output.cpp $(OPENMPFLAG)

run: wave2d
	./wave2d waveparamsnc.txt

runwithcores: runwithcores
	$(CXX) -o runwithcores runwithcores.cpp

runall: runall
	make runwithcores 
	$(CXX) -o runall runall.cpp


clean:
	$(RM) wave2d.o parameters.o initialize.o output.o evolve.o simulation.o timing_output.o wave2d

fullclean: clean
	$(RM) wave2d gmon.out runwithcores runall

.PHONY: all clean run fullclean

