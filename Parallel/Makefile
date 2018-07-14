CXX=g++
PAR_CXX=mpiCC
CXXFLAGS=-std=c++0x -Wall -W
BUGFLAGS=-g
PROFFLAG=-pg

objects = Agent_par.o Environment_par.o kd_tree.o mainPedestrian.o
par_objects = Agent_par.o Environment_par.o kd_tree.o mainParallel.o

all: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) -o pedestrian

parallel: $(par_objects)
	$(PAR_CXX) $(par_objects) -o ped_parallel

debug: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) $(BUGFLAGS) -o pedestrian

prof: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) $(PROFFLAG) -o pedestrian

Agent_par.o : Agent_par.h kd_tree.h
Environment_par.o : Environment_par.h
kd_tree.o : kd_tree.h Agent_par.h
mainPedestrian.o : Agent_par.h kd_tree.h Environment_par.h
mainParallel.o : mainParallel.cc Agent_par.h kd_tree.h Environment_par.h
	$(PAR_CXX) $(CFLAGS) -c mainParallel.cc -o mainParallel.o

#Agent_par.o : Agent_par.cc Agent_par.h kd_tree.h
#	$(CC) $(CFLAGS) -c Agent_par.cc -o Agent_par.o
#Environment_par.o : Environment_par.cc Environment_par.h
#	$(CC) $(CFLAGS) -c Environment_par.cc -o Environment_par.o
#kd_tree.o : kd_tree.cc kd_tree.h Agent_par.h
#	$(CC) $(CFLAGS) -c kd_tree.cc -o kd_tree.o
#mainPedestrian.o : mainPedestrian.cc Agent_par.h kd_tree.h Environment_par.h
#	$(CC) $(CFLAGS) -c mainPedestrian.cc -o mainPedestrian.o
#mainParallel.o : mainParallel.cc Agent_par.h kd_tree.h Environment_par.h
#	$(PAR_CC) $(CFLAGS) -c mainParallel.cc -o mainParallel.o

#debug: Agent.cc Environment.cc visual_grid.cc mainPedestrian.cc
#	$(CC) Agent.cc Environment.cc visual_grid.cc mainPedestrian.cc $(CFLAGS) $(BUGFLAG) -o pedestrian

.PHONY : clean
clean:
	rm -f pedestrian ped_parallel $(objects) $(par_objects)
