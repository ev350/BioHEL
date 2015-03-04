#include "populationWrapper.h"
#include "windowingILAS.h"
#include "configManagement.h"
#include "attributesInfo.h"
#include "random.h"
#include "classifierFitness.h"
#include "messageBuffer.h"
#include <stdlib.h>

extern messageBuffer mb;
extern Random rnd;
extern attributesInfo ai;
extern configManagement cm;
extern int nodeRank;

void windowingILAS::setInstances(instance **pSet,int pHowMuch)
{
	if(strata!=NULL) {
		if(pHowMuch>howMuch) {
			delete strata;
			strata = new instance *[pHowMuch];
		}
	} else {
		strata = new instance *[pHowMuch];
	}

	set=pSet;
	howMuch=pHowMuch;
	currentIteration=0;
	if(cm.thereIsParameter(DOB_PARTITIONS_IN_WINDOWING)) {
		//reorderInstancesDOB_approx();
		reorderInstancesDOB();
	} else {
		reorderInstances();
	}
}

windowingILAS::windowingILAS()
{
	numStrata=(int)round(cm.getParameter(WINDOWING_ILAS));
	strataSizes = new int[numStrata];
	strataOffsets = new int[numStrata];
	strata=NULL;
}

windowingILAS::~windowingILAS()
{
	delete strata;
	delete strataSizes;
	delete strataOffsets;
}

void windowingILAS::reorderInstancesDOB_approx()
{
	int i,j,k;
	int nc=ai.getNumClasses();

	instance ***setsPerClass = new instance **[nc];
	int countsClass[nc];
	for(i=0;i<nc;i++) {
		setsPerClass[i] = new instance *[howMuch];
		countsClass[i]=0;
	}

	for(i=0;i<howMuch;i++) {
		int cls=set[i]->getClass();
		setsPerClass[cls][countsClass[cls]++]=set[i];
	}

	int tempCapacity=howMuch/numStrata+nc;
	int *countTemp = new int[numStrata];
	instance ***tempStrata = new instance **[numStrata];
	for(i=0;i<numStrata;i++) {
		countTemp[i]=0;
		tempStrata[i]= new instance *[tempCapacity];
	}
	
	for(i=0;i<nc;i++) {
		int num = countsClass[i];
		while(num>numStrata) {
			//mb.printf("Assigning instances of class %d. Instances left: %d\n",i,num);
			Sampling sampClass(num);
			int numSamples = numStrata * 10;
			if(numSamples > num ) numSamples = num;
			float distances[numSamples];
			int pos[numSamples];
			int seed = rnd(0,num-1);

			for(j = 0 ; j < numSamples ; j++) {
				int posS = sampClass.getSample();
				distances[j] = setsPerClass[i][seed]->distance(setsPerClass[i][posS]);
				pos[j] = posS;
			}

			int selected[numStrata];
			selected[0] = seed;
			for(j=1; j < numStrata ; j++) {
				float best = distances[0];
				int posBest = 0;
				for(k=1; k<numSamples-j+1;k++) {
					if(distances[k]<best) {
						posBest = k;
						best = distances[k];
					}
				}
				selected[j] = pos[posBest];
				distances[posBest] = distances[numSamples-j];
				pos[posBest] = pos[numSamples-j];
			}
					

			Sampling samp(numStrata);
			for(j=0; j<numStrata; j++) {
				int posS = selected[samp.getSample()];
				tempStrata[j][countTemp[j]++] = setsPerClass[i][posS];
			}

			for(j=0; j<numStrata; j++) {
				setsPerClass[i][selected[j]] = setsPerClass[i][num-j-1];
			}
			num-=numStrata;
		} 

		if(num>0) {
			Sampling samp(numStrata);
			for(j=0; j<num; j++) {
				int stratum = samp.getSample();
				tempStrata[stratum][countTemp[stratum]++] = setsPerClass[i][j];
			}
		} 

		delete [] setsPerClass[i];
	}


	int acum=0;
	for(i=0;i<numStrata;i++) {
		int size=countTemp[i];
		strataSizes[i]=size;
		strataOffsets[i]=acum;
		for(j=0;j<size;j++) {
			strata[acum++]=tempStrata[i][j];
		}
	}

	for(i=0;i<numStrata;i++) {
		delete [] tempStrata[i];
	}
	delete [] countTemp;
	delete [] tempStrata;

	delete [] setsPerClass;
}


void windowingILAS::reorderInstancesDOB()
{
	int i,j,k;
	int nc=ai.getNumClasses();

	instance ***setsPerClass = new instance **[nc];
	int countsClass[nc];
	for(i=0;i<nc;i++) {
		setsPerClass[i] = new instance *[howMuch];
		countsClass[i]=0;
	}

	for(i=0;i<howMuch;i++) {
		int cls=set[i]->getClass();
		setsPerClass[cls][countsClass[cls]++]=set[i];
	}

	int tempCapacity=howMuch/numStrata+nc;
	int *countTemp = new int[numStrata];
	instance ***tempStrata = new instance **[numStrata];
	for(i=0;i<numStrata;i++) {
		countTemp[i]=0;
		tempStrata[i]= new instance *[tempCapacity];
	}
	
	for(i=0;i<nc;i++) {
		int num = countsClass[i];
		while(num>numStrata) {
			//mb.printf("Assigning instances of class %d. Instances left: %d\n",i,num);
			float distances[numStrata];
			int pos[numStrata];
			int seed = rnd(0,num-1);
			distances[0] = setsPerClass[i][seed]->distance(setsPerClass[i][0]);
			pos[0]=0;
			int worst=0;
			float distWorst=distances[0];

			for(j=1; j<numStrata; j++) {
				distances[j] = setsPerClass[i][seed]->distance(setsPerClass[i][j]);
				pos[j]=j;
				if(distances[j]>distWorst) {
					distWorst=distances[j];
					worst = j;
				}
			}

			for(j = numStrata ; j<num; j++) {
				float dist = setsPerClass[i][seed]->distance(setsPerClass[i][j]);
				if(dist<distWorst) {
					pos[worst] = j;
					distances[worst] = dist;
			
					worst=0;
					distWorst=distances[0];
					for(k=1; k<numStrata; k++) {
						if(distances[k]>distWorst) {
							distWorst=distances[k];
							worst = k;
						}
					}
				}
			}

			Sampling samp(numStrata);
			for(j=0; j<numStrata; j++) {
				int posS = pos[samp.getSample()];
				tempStrata[j][countTemp[j]++] = setsPerClass[i][posS];
			}

			for(j=0; j<numStrata; j++) {
				setsPerClass[i][pos[j]] = setsPerClass[i][num-j-1];
			}
			num-=numStrata;
		} 

		if(num>0) {
			Sampling samp(numStrata);
			for(j=0; j<num; j++) {
				int stratum = samp.getSample();
				tempStrata[stratum][countTemp[stratum]++] = setsPerClass[i][j];
			}
		} 

		delete setsPerClass[i];
	}


	int acum=0;
	for(i=0;i<numStrata;i++) {
		int size=countTemp[i];
		strataSizes[i]=size;
		strataOffsets[i]=acum;
		for(j=0;j<size;j++) {
			strata[acum++]=tempStrata[i][j];
		}
	}

	for(i=0;i<numStrata;i++) {
		delete [] tempStrata[i];
	}
	delete [] countTemp;
	delete [] tempStrata;

	delete [] setsPerClass;
}


void windowingILAS::reorderInstances()
{
    int i,j;
	int nc=ai.getNumClasses();

	Sampling **samplings = new Sampling *[nc];
	for(i=0;i<nc;i++) {
		samplings[i] = new Sampling(numStrata);
	}

	int tempCapacity=howMuch/numStrata+nc;
	int *countTemp = new int[numStrata];
	instance ***tempStrata = new instance **[numStrata];
	for(i=0;i<numStrata;i++) {
		countTemp[i]=0;
		tempStrata[i]= new instance *[tempCapacity];
	}

	for(i=0;i<howMuch;i++) {
		int cls=set[i]->getClass();
		int str=samplings[cls]->getSample();
		tempStrata[str][countTemp[str]++]=set[i];
	}

	int acum=0;
	for(i=0;i<numStrata;i++) {
		int size=countTemp[i];
		strataSizes[i]=size;
		strataOffsets[i]=acum;
		for(j=0;j<size;j++) {
			strata[acum++]=tempStrata[i][j];
		}
	}

	for(i=0;i<numStrata;i++) {
		delete [] tempStrata[i];
	}
	delete [] countTemp;
	delete [] tempStrata;

	for(i=0;i<nc;i++) {
		delete samplings[i];
	}
	delete [] samplings;
}

void windowingILAS::newIteration(instance **&selectedInstances,int &numSelected) 
{
	stratum=currentIteration%numStrata;
	numSelected=strataSizes[stratum];
	selectedInstances=&strata[strataOffsets[stratum]];
	currentIteration++;
}

