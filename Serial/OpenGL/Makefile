CXX=g++
CXXFLAGS=-std=c++0x
BUGFLAG=-g -pg
LIBGRAPHICS = -lGL -lGLU -lglut

objects = Agent.o Environment.o visual_grid.o mainPedestrian.o

all: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) $(LIBGRAPHICS) -o pedestrian

debug: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) $(BUGFLAGS) -o pedestrian

Agent.o : Agent.h Environment.h
Environment.o : Environment.h 
visual_grid.o : visual_grid.h
mainPedestrian.o : Agent.h visual_grid.h

.PHONY : clean
clean:
	rm -f pedestrian  $(objects)
