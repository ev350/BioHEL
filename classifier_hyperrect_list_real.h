#ifndef _CLASSIFIER_HYPERRECT_LIST_REAL_
#define _CLASSIFIER_HYPERRECT_LIST_REAL_

#include "classifier.h"
#include "timerRealKR.h"
#include "timerGlobals.h"
#include "agentPerformanceTraining.h"

extern timerRealKR *tReal;
extern timerGlobals *tGlobals;

class classifier_hyperrect_list_real: public classifier   {

	void crossover_1px(classifier_hyperrect_list_real *in1,classifier_hyperrect_list_real *in2
		,classifier_hyperrect_list_real *out1,classifier_hyperrect_list_real *out2);
	float mutationOffset(float geneValue,float offsetMin,float offsetMax);
	void initializeChromosome(void);

public:
	float *predicates;
	int *offsetPredicates;
	int numAtt;
	int *whichAtt;
	int classValue;
	int ruleSize;

	classifier_hyperrect_list_real();
	classifier_hyperrect_list_real(const classifier_hyperrect_list_real &orig,int son=0);
	~classifier_hyperrect_list_real();

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
			if(value<predicates[base] || value>predicates[base+1]) return 0;
		}
		return 1;
	}

	double computeTheoryLength();
	void crossover(classifier *,classifier *,classifier *);
	void mutation();
	void dumpPhenotype(char *string);

	inline int numSpecialStages(){return 2;}
	void doSpecialStage(int);

	void postprocess();
	int getSpecificity(int *indexes,double *specificity);
};

#endif

