#CC=g++ -03
CC=g++ -w
CFLAGS=-g -Wall -std=c++11
CPPFLAGS=-I include
LIBS = -lGL -lglut -lGLU -lSOIL

all: build drone

## Compile with -pg option to profile with gprof
debug: build/neat.o build/network.o build/nnode.o build/link.o build/trait.o build/gene.o build/genome.o build/innovation.o build/organism.o build/species.o build/population.o build/experiments.o build/neatmain.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -pg $^ -o neat ## removed -c 
####

drone: build/neat.o build/drone.o build/network.o build/nnode.o build/link.o build/trait.o build/gene.o build/genome.o build/innovation.o build/organism.o build/species.o build/population.o build/drone-evaluation.o build/main.o
	
	$(CC) $(CFLAGS) $(CPPFLAGS)  $^ -o $@ $(LIBS) ## removed -c 

build/neat.o: src/neat.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/network.o: src/network.cpp  build/neat.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/nnode.o: src/nnode.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/link.o: src/link.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/trait.o: src/trait.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/gene.o: src/gene.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/genome.o: src/genome.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/innovation.o: src/innovation.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/organism.o: src/organism.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/species.o: src/species.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/population.o: src/population.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/drone-evaluation.o: src/drone-evaluation.cpp  build/drone.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LIBS)

build/drone.o:  src/drone.cpp build/main.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LIBS)

build/main.o: src/main.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LIBS)

build:
	mkdir build

clean:
	rm -rf build/ *~

purge:
	@echo Deleting *.o, *~, ./build and ./ndrone
	rm -rf build *~ drone
