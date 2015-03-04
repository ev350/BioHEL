#include "populationWrapper.h"
#include <stdio.h>

populationWrapper::populationWrapper(int pPopSize)
{
	cf=new classifierFactory;
	ga=new geneticAlgorithm(cf);
	popSize = pPopSize;
}

populationWrapper::~populationWrapper()
{
	delete ga;
	delete cf;
}

void populationWrapper::activateModifiedFlag()
{
	int i;

	rankP *rk=ga->getPopulationRank();
	for(i=0;i<popSize;i++) rk[i].ind->activateModified();
	ga->resetBest();
}

rankP *populationWrapper::getPopulationRank()
{
	return ga->getPopulationRank();
}


void populationWrapper::gaIteration()
{
	ga->doIterations(1);
}

void populationWrapper::releasePopulation()
{
	ga->destroyPopulation();
}

classifier *populationWrapper::getBestOverall()
{
	return (classifier *)ga->getBest();
}

classifier **populationWrapper::getPopulation()
{
	return (classifier **)ga->getPopulation();
}


classifier *populationWrapper::getBestPopulation()
{
	rankP *rk=ga->getPopulationRank();
	return (classifier *)rk[0].ind;
}

classifier *populationWrapper::getWorstPopulation()
{
	rankP *rk=ga->getPopulationRank();
	return (classifier *)rk[popSize - 1].ind;
}

double populationWrapper::getAverageLength()
{
	int i;
	double ave=0;

	rankP *rk=ga->getPopulationRank();

	for(i=0;i<popSize;i++) ave+=rk[i].ind->getLength();

	return ave/(double)popSize;
}

void populationWrapper::getAverageDevAccuracy(double &ave,double &dev)
{
	int i;
	ave=0;
	dev=0;

	rankP *rk=ga->getPopulationRank();

	for(i=0;i<popSize;i++) {
		double acc=((classifier *)rk[i].ind)->getAccuracy();
		ave+=acc;
		dev+=(acc*acc);
	}
	dev-=(ave*ave)/(double)popSize;
	dev/=(double)(popSize-1);
	dev=sqrt(dev);
	ave/=(double)popSize;
}

void populationWrapper::getAverageAccuracies(double &ave1,double &ave2)
{
	int i;
	ave1=0;
	ave2=0;
	rankP *rk=ga->getPopulationRank();

	for(i=0;i<popSize;i++) {
		ave1+=((classifier *)rk[i].ind)->getAccuracy();
		ave2+=((classifier *)rk[i].ind)->getAccuracy2();
	}

	ave1/=(double)popSize;
	ave2/=(double)popSize;
}


double populationWrapper::getMaxAccuracy()
{
	int i;

	rankP *rk=ga->getPopulationRank();
	double max=((classifier *)rk[0].ind)->getAccuracy();

	for(i=1;i<popSize;i++) {
		double percen=((classifier *)rk[i].ind)->getAccuracy();
		if(percen>max) max=percen;
	}

	return max;
}

classifier *populationWrapper::cloneClassifier(classifier *orig,int son)
{
	return cf->cloneClassifier(orig,son);
}

classifier *populationWrapper::createClassifier()
{
	return cf->createClassifier();
}

void populationWrapper::destroyClassifier(classifier *orig)
{
	cf->deleteClassifier(orig);
}



void populationWrapper::dumpAttStatsBest(double *stats)
{
	int i, j, index, k = tGlobals->numAttributesMC;
	classifier *ind = NULL;

	rankP *rk = ga->getPopulationRank();
	int *attIndex = new int[k];
	double *specificity = new double[k];
	for (i = 0; i < 3 * k; i++) {
		 stats[i] = 0;
	}

	ind = rk[0].ind;
	int numAtt=ind->getSpecificity(attIndex,specificity);

	for (j = 0; j < numAtt; j++) {
		 index = attIndex[j];
		 stats[index] = ind->getAccuracy2();
		 stats[k + index] = ind->getCoverage();
		 stats[2 * k + index] = specificity[j];
	}

	delete attIndex;
	delete specificity;
}

