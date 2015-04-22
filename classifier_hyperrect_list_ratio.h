#ifndef _CLASSIFIER_HYPERRECT_LIST_ratio_
#define _CLASSIFIER_HYPERRECT_LIST_ratio_

#include <vector>
#include "classifier.h"
#include "timerRealKR.h"
#include "timerGlobals.h"
#include "agentPerformanceTraining.h"
#include "ratio_test.h"

// try object-oriented approach

extern timerRealKR *tReal;
extern timerGlobals *tGlobals;

class classifier_hyperrect_list_ratio: public classifier   {
    
    void crossover_1px(classifier_hyperrect_list_ratio *in1,classifier_hyperrect_list_ratio *in2
                       ,classifier_hyperrect_list_ratio *out1,classifier_hyperrect_list_ratio *out2);
    void initializeChromosome(void);
    
public:
    std::vector<ratio_test> *rule;
    int classValue;
    
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
        for(int i = 0; i < rule->size(); i++) {
            if (!rule->at(i).isTrue(ins))
                return false;
        }
        return true;
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
