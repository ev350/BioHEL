#include "timerCrossover.h"

extern timerCrossover *tCross;

void geneticAlgorithm::replacementAlgorithm()
{
	totalReplacement();
	doFitnessComputations();
	createPopulationRank();
	if(tGlobals->elitismEnabled) doElitism();
}

void geneticAlgorithm::doElitism()
{
	int i,j;

	for(i=0;i<numVersions;i++) {
		if(best[i]) {
			best[i]->fitnessComputation();
		}
	}

	int numV=numVersions;
	JVector<int> priorities(popSize+numV);
	for(i=0;i<popSize;i++) priorities.addElement(populationRank[i].pos);
	for(i=0;i<numV;i++) {
		if(best[i]) {
			int size=priorities.size();
			for(j=0;j<size;j++) {
				classifier *ind;
				int pos=priorities[j];
				if(pos>=popSize) {
					ind=best[pos-popSize];
				} else {
					ind=population[pos];
				}

				if(best[i]->compareToIndividual(ind,optimizationMethod)>0) {
					priorities.insertElementAt(popSize+i,j);
					break;
				}
			}
			if(j==size) {
				priorities.addElement(popSize+i);
			}
		}
	}

	JVector<int> elite;
	for(i=0;i<popSize;i++) {
		if(priorities[i]>=popSize) {
			//mb.printf("Elite element %d enters the population\n",priorities[i]-popSize);
			elite.addElement(priorities[i]-popSize);
		}
	}
	int index=0;
	int size=priorities.size();
	for(i=popSize;i<size;i++) {
		if(priorities[i]<popSize) {
			int pos=priorities[i];
			cf->deleteClassifier(population[pos]);
			population[pos]= cf->cloneClassifier(best[elite[index++]]);
		}
	}
	flagResetBest=0;
}


void geneticAlgorithm::totalReplacement()
{
	int i;

	for(i=0;i<popSize;i++) {
		cf->deleteClassifier(population[i]);
	}

	classifier **tempPop=population;
	population = offspringPopulation;
	offspringPopulation=tempPop;
}
