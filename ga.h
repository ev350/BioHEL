#ifndef _GA_
#define _GA_

#include "factory.h"
#include "random.h"
#include "instanceSet.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
        int pos;
        classifier *ind;
} rankP;

extern instanceSet *is;


class geneticAlgorithm {
	int currentIteration;
	int popSize;
	classifierFactory *cf;
	classifier **population, **offspringPopulation;
	rankP *populationRank;
	int flagResetBest;
	int numVersions;
	classifier **best;

	void doFitnessComputations();
	void checkBestIndividual();
	void createPopulationRank();

	void initializePopulation();

#include "crossover.h"
#include "scaling.h"
#include "replacement.h"
#include "selection.h"
#include "mutation.h"

      public:
	void destroyPopulation();
	geneticAlgorithm(classifierFactory *cf);
	~geneticAlgorithm();
	void doIterations(int num);
	classifier **getPopulation() { return population; }
	classifier *getBest() { return best[is->getCurrentVersion()]; }
	classifier *getWorst() {
		return population[populationRank[popSize - 1].pos];
	}
	rankP *getPopulationRank() { return populationRank; }
	void resetBest();
};

#endif
