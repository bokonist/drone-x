#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include "neat.h"
#include "network.h"
#include "population.h"
#include "organism.h"
#include "genome.h"
#include "species.h"

NEAT::Population *drone_test(int gens);
bool drone_evaluate(NEAT::Organism *org);
int drone_epoch(NEAT::Population *pop,int generation,char *filename, int &winnernum, int &winnergenes,int &winnernodes);