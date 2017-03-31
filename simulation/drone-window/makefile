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

build/neat.o: src/neat.cpp include/neat.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/network.o: src/network.cpp include/network.h include/neat.h build/neat.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/nnode.o: src/nnode.cpp include/nnode.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/link.o: src/link.cpp include/link.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/trait.o: src/trait.cpp include/trait.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/gene.o: src/gene.cpp include/gene.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/genome.o: src/genome.cpp include/genome.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/innovation.o: src/innovation.cpp include/innovation.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/organism.o: src/organism.cpp include/organism.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/species.o: src/species.cpp include/species.h include/organism.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/population.o: src/population.cpp include/population.h include/organism.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/drone-evaluation.o: src/drone-evaluation.cpp include/drone-evaluation.h include/network.h include/species.h build/drone.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LIBS)

build/drone.o: include/drone.h src/drone.cpp build/main.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LIBS)

build/main.o: src/main.cpp include/neat.h include/population.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LIBS)

build:
	mkdir build

clean:
	rm -rf build/ *~

purge:
	@echo Deleting *.o, *~, ./build and ./ndrone
	rm -rf build *~ drone