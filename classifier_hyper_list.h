//
//  classifier_hyper_list.h
//  BioHEL
//
//  Created by Euan Cowie on 23/04/2015.
//
//

#ifndef BioHEL_classifier_hyper_list_h
#define BioHEL_classifier_hyper_list_h

#include <vector>
#include "test.h"
#include "classifier.h"
#include "timerRealKR.h"
#include "timerGlobals.h"
#include "agentPerformanceTraining.h"

extern timerRealKR *tReal;
extern timerGlobals *tGlobals;

class classifier_hyper_list: public classifier   {
    
    virtual void crossover_1px(classifier_hyper_list *in1, classifier_hyper_list *in2, classifier_hyper_list *out1, classifier_hyper_list *out2);
    virtual void initializeChromosome(void);
    
public:
    std::vector<test*> *rule;
    test::Type type;
    int classValue;
    
    
    classifier_hyper_list(test::Type type);
    classifier_hyper_list(const classifier_hyper_list &orig, int son=0);
    virtual ~classifier_hyper_list();
    
    void swapD(float &a, float &b) {
        float temp=a;
        a=b;
        b=temp;
    }
    
    int getClass() {
        return classValue;
    }
    
    int doMatch(instance * ins)
    {
        for(int i = 0; i < rule->size(); i++) {
            if (!rule->at(i)->isTrue(ins))
                return false;
        }
        return true;
    }
    
    inline static int binarySearch(std::vector<test*> rule, int from, int to, int key) {
        int low = from;
        int high = to - 1;
        
        while (low <= high) {
            int mid = (low + high) >> 1;
            int midVal = rule[mid]->attribute;
            
            if (midVal < key)
                low = mid + 1;
            else if (midVal > key)
                high = mid - 1;
            else
                return mid; // key found
        }
        return low + 1;  // key not found.
    }
    
    virtual double computeTheoryLength();
    virtual void crossover(classifier *, classifier *, classifier *);
    virtual void mutation();
    virtual void dumpPhenotype(char *string);
    
    int numSpecialStages() { return 2; }
    virtual int getSpecificity(int *indexes, double *specificity);
    virtual void doSpecialStage(int);
    
    virtual void postprocess();
};


#endif
