#include "ga.h"
#include "configManagement.h"
#include "timerGlobals.h"
#include "random.h"
#include "timeManagement.h"
#include "messageBuffer.h"
#include "timerGlobals.h"

extern Random rnd;
extern timeManagement tm;
extern configManagement cm;
static int optimizationMethod;
extern int lastIteration;
extern messageBuffer mb;
extern int numTasks;
extern timerGlobals *tGlobals;

//Including a .cpp is ugly but ...
#include "crossover.cpp"
#include "replacement.cpp"
#include "selection.cpp"
#include "scaling.cpp"
#include "mutation.cpp"

int rankOrder(const void *pA, const void *pB)
{
	rankP *a=(rankP *)pA;
	rankP *b=(rankP *)pB;

	return a->ind->compareToIndividual2(b->ind, optimizationMethod);
}


void geneticAlgorithm::createPopulationRank()
{
	int i;

	for (i = 0; i < popSize; i++) {
		populationRank[i].pos = i;
		populationRank[i].ind = population[i];
	}
	qsort(populationRank, popSize,sizeof(rankP),rankOrder);
}

void geneticAlgorithm::initializePopulation()
{
	int i;

	popSize = (int) cm.getParameter(POP_SIZE);

	population = new classifier *[popSize];
	offspringPopulation = new classifier *[popSize];
	if (!population || !offspringPopulation) {
		perror("out of memory");
		exit(1);
	}

	for (i = 0; i < popSize; i++) {
		population[i] = cf->createClassifier();
		if (!population[i]) {
			perror("out of memory");
			exit(1);
		}
	}

	populationRank = new rankP[popSize];
	flagResetBest=0;
	currentIteration = 0;
}

void geneticAlgorithm::doFitnessComputations()
{
	int i;

	for(i=0;i<popSize;i++) {
		population[i]->fitnessComputation();
	}

	maxFitness=minFitness=population[0]->getFitness();
	for(i=1;i<popSize;i++) {
		double fitness=population[i]->getFitness();
		if(fitness>maxFitness) {
			maxFitness=fitness;
		}
		if(fitness<minFitness) {
			minFitness=fitness;
		}
	}
}

void geneticAlgorithm::resetBest()
{
	flagResetBest=1;
}

void geneticAlgorithm::checkBestIndividual()
{
	int currVer=is->getCurrentVersion();

	if (best[currVer]==NULL) {
		best[currVer] = cf->cloneClassifier(populationRank[0].ind);
	} else {
		best[currVer]->fitnessComputation();
		if (best[currVer]->compareToIndividual(populationRank[0].ind, 
			optimizationMethod) < 0) {
			//mb.printf("Best indiv %d replaced\n",currVer);
			cf->deleteClassifier(best[currVer]);
			best[currVer] = cf->cloneClassifier(populationRank[0].ind);
		}
	}
}

void geneticAlgorithm::destroyPopulation()
{
	int i;

	for (i = 0; i < popSize; i++) cf->deleteClassifier(population[i]);
	delete population;
	delete populationRank;
	delete offspringPopulation;
}

geneticAlgorithm::geneticAlgorithm(classifierFactory *pCF)
{
	cf = pCF;

        optimizationMethod = (int) cm.getParameter(MAX_MIN);
        selectionAlg = (int) cm.getParameter(SELECTION_ALGORITHM);
	tournamentSize = (int) cm.getParameter(TOURNAMENT_SIZE);
	showFronts = cm.thereIsParameter(SHOW_FRONTS);

	numVersions=is->numVersions();
	best = new classifier *[numVersions+1];
	for(int i=0;i<numVersions;i++) best[i]=NULL;

	initializePopulation();
	doFitnessComputations();
	createPopulationRank();
	checkBestIndividual();
}

geneticAlgorithm::~geneticAlgorithm()
{
	destroyPopulation();
	for(int i=0;i<numVersions;i++) 
		if(best[i]) cf->deleteClassifier(best[i]);
	delete best;
}

void geneticAlgorithm::doIterations(int n)
{
	for (; n > 0; n--) {
		//tm.startChronometer();

		selectionAlgorithm();
		crossover();
		mutation();

		//tm.stopChronometer();
		//tm.addTimeToStage("alpha");

		replacementAlgorithm();

		// We already call fitnessComputation in replacement
		//fitnessComputation();
		//tm.startChronometer();

		createPopulationRank();
		checkBestIndividual();
		currentIteration++;

		//tm.stopChronometer();
		//tm.addTimeToStage("alpha");
	}
}

