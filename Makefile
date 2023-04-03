# makefile for the wave2d application

CXX=g++

#DBGFLAGS=-g
OPTFLAGS= -O2 -march=native
GPROFFLAGS= -pg -g -gstabs
CXXFLAGS=$(OPTFLAGS) $(DBGFLAGS) $(GPROFFLAGS) -std=c++17 -Wall -Wfatal-errors -Wconversion -Wno-narrowing -Wno-sign-compare -isystem ${MODULE_NETCDF_PREFIX}/include
LDLIBS=-lnetcdf -lnetcdf_c++4
LDFLAGS=$(OPTFLAGS) $(GPROFFLAGS) $(DBGFLAGS)

all: wave2d

wave2d: wave2d.o parameters.o initialize.o output.o evolve.o simulation.o
	$(CXX) $(LDFLAGS) -o $@ $^  $(LDLIBS)

wave2d.o: wave2d.cpp parameters.h wavetypes.h initialize.h output.h evolve.h simulation.h
	$(CXX) -c $(CXXFLAGS) -o wave2d.o wave2d.cpp

parameters.o: parameters.cpp parameters.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o parameters.o parameters.cpp

initialize.o: initialize.cpp initialize.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o initialize.o initialize.cpp

output.o: output.cpp output.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o output.o output.cpp

evolve.o: evolve.cpp evolve.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o evolve.o evolve.cpp

simulation.o: simulation.cpp simulation.h wavetypes.h
	$(CXX) -c $(CXXFLAGS) -o simulation.o simulation.cpp

run: wave2d
	./wave2d waveparamsnc.txt

clean:
	$(RM) wave2d.o parameters.o initialize.o output.o evolve.o simulation.o 

fullclean: clean
	$(RM) wave2d gmon.out

.PHONY: all clean run fullclean

