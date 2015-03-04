#ifndef _classifier_hyperrect_list_less_
#define _classifier_hyperrect_list_less_

#include "classifier.h"
#include "timerRealKR.h"
#include "timerGlobals.h"
#include "agentPerformanceTraining.h"

extern timerRealKR *tReal;
extern timerGlobals *tGlobals;

class classifier_hyperrect_list_less: public classifier   {

	void crossover_1px(classifier_hyperrect_list_less *in1,classifier_hyperrect_list_less *in2
		,classifier_hyperrect_list_less *out1,classifier_hyperrect_list_less *out2);
	float mutationOffset(float geneValue,float offsetMin,float offsetMax);
	void initializeChromosome(void);

public:
	float *predicates;
	int *offsetPredicates;
	int numAtt;
	int *whichAtt;
	int classValue;
	int ruleSize;

	classifier_hyperrect_list_less();
	classifier_hyperrect_list_less(const classifier_hyperrect_list_less &orig,int son=0);
	~classifier_hyperrect_list_less();

	inline void swapD(float &a,float &b) {
		float temp=a;
		a=b;
		b=temp;
	}


	inline int getClass() {
		return classValue;
	}

	inline int doMatch(instance * ins)
	{
		int i,base;

		for(i=0;i<numAtt;i++) {
			base=offsetPredicates[i];
			int att=whichAtt[i];
			register float value=ins->realValues[att];
			if(value>predicates[base+1]) return 0;
			//if(value<predicates[base] || value>predicates[base+1]) return 0;
		}
		return 1;
	}

	double computeTheoryLength();
	void crossover(classifier *,classifier *,classifier *);
	void mutation();
	void dumpPhenotype(char *string);

	inline int numSpecialStages(){return 2;}
	int getSpecificity(int *indexes,double *specificity);
	void doSpecialStage(int);

	void postprocess();
};

#endif

