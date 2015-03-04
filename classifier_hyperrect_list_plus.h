#ifndef _CLASSIFIER_HYPERRECT_LIST_PLUS_
#define _CLASSIFIER_HYPERRECT_LIST_PLUS_

#include <vector>
#include "classifier.h"
#include "timerRealKR.h"
#include "timerGlobals.h"
#include "agentPerformanceTraining.h"
#include "greater_than_test.h"

// try object-oriented approach

extern timerRealKR *tReal;
extern timerGlobals *tGlobals;

class classifier_hyperrect_list_plus: public classifier   {
    
    void crossover_1px(classifier_hyperrect_list_plus *in1,classifier_hyperrect_list_plus *in2
                       ,classifier_hyperrect_list_plus *out1,classifier_hyperrect_list_plus *out2);
    float mutationOffset(float geneValue,float offsetMin,float offsetMax);
    void initializeChromosome(void);
    
public:
//    float *predicates; //contain thresholds - upper/lower
//    int *offsetPredicates; //position in the predicate array where the data for each att starts
    int numAtt; //number of attributes included in the rule
//    int *whichAtt; //list of the attributes (indexes)
    int classValue;
    int ruleSize; //size of the predicate array
    
    std::vector<greater_than_test> rule;
    
    classifier_hyperrect_list_plus();
    classifier_hyperrect_list_plus(const classifier_hyperrect_list_plus &orig,int son=0);
    ~classifier_hyperrect_list_plus();
    
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
        for(int i = 0; i < rule.size(); i++) {
            if (!rule.at(i).isTrue())
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
