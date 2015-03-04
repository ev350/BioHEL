#ifndef _CLASSIFIER_HYPERRECT_LIST_ratio_
#define _CLASSIFIER_HYPERRECT_LIST_ratio_

#include "classifier.h"
#include "timerRealKR.h"
#include "timerGlobals.h"
#include "agentPerformanceTraining.h"

extern timerRealKR *tReal;
extern timerGlobals *tGlobals;

class classifier_hyperrect_list_ratio: public classifier   {

	void crossover_1px(classifier_hyperrect_list_ratio *in1,classifier_hyperrect_list_ratio *in2
		,classifier_hyperrect_list_ratio *out1,classifier_hyperrect_list_ratio *out2);
	float mutationOffset(float geneValue,float offsetMin,float offsetMax);
	void initializeChromosome(void);

public:
	float *predicates;
	int *offsetPredicates;
	int numAtt;
	int *whichAtt;
	int classValue;
	int ruleSize;

	classifier_hyperrect_list_ratio();
	classifier_hyperrect_list_ratio(const classifier_hyperrect_list_ratio &orig,int son=0);
    ~classifier_hyperrect_list_ratio();

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

		for(i = 0; i < numAtt; ++i) {
			base = offsetPredicates[i];
			int att = whichAtt[i];

			register float value = ins->realValues[att];

			if (value < (predicates[base] / predicates[base + 1])) return 0;
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
