#include "classifier_hyperrect_list_ratio.h"
#include "random.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <sstream>
#include "attributesInfo.h"
#include "timerGlobals.h"
#include "timerMutation.h"
#include "timerCrossover.h"
#include "timerMDL.h"
#include "timerHierar.h"
#include "timerRealKR.h"
#include "instanceSet.h"
#include "utils.h"

using namespace std;

extern attributesInfo ai;
extern timerGlobals *tGlobals;
extern timerHierar *tHierar;
extern timerMutation *tMut;
extern timerCrossover *tCross;
extern timerMDL *tMDL;
extern timerRealKR *tReal;
extern instanceSet *is;
extern Random rnd;

double classifier_hyperrect_list_ratio::computeTheoryLength()
{
    int i;
    theoryLength = 0.0;
    
    for(i = 0; i < rule->size(); i++) {
        theoryLength += rule->at(i).computeLength();
    }
    
    theoryLength /= (double)tGlobals->numAttributesMC;
    
    return theoryLength;
}

classifier_hyperrect_list_ratio::classifier_hyperrect_list_ratio()
{
    classifier_hyperrect_list_ratio::initializeChromosome();
}

//
classifier_hyperrect_list_ratio::classifier_hyperrect_list_ratio(const classifier_hyperrect_list_ratio & orig, int son)
{
    *this = orig;
    
    if (!son) {
        rule = new vector<ratio_test>(*orig.rule);
    }
}

classifier_hyperrect_list_ratio::~classifier_hyperrect_list_ratio()
{
    
}

void classifier_hyperrect_list_ratio::initializeChromosome()
{
    //    cout << "Initialize Chromosome" << endl;
    
    int i;
    
    instance *ins = NULL;
    if(tGlobals->smartInit) {
        if(tGlobals->defaultClassPolicy != DISABLED) {
            ins = is->getInstanceInit(tGlobals->defaultClass);
        } else {
            ins = is->getInstanceInit(ai.getNumClasses());
        }
    }
    
    JVector<int> selectedAtts;
    
    if(tReal->fixExpAtt) {
        for(i = 0; i < tReal->numExpAtt; i++) {
            int selected = tReal->sampAtts->getSample();
            selectedAtts.addElement(selected);
        }
        sortList(selectedAtts);
    } else {
        for (i = 0; i < tGlobals->numAttributesMC; i++) {
            if(!rnd >= tReal->probIrr) {
                selectedAtts.addElement(i);
            }
        }
    }
    
    rule = new vector<ratio_test>();
    
    for(i = 0; i < selectedAtts.size(); i++) {
        rule->push_back(ratio_test(selectedAtts[i], selectedAtts[i], ins));
    }
    
    if(ins) {
        classValue = ins->getClass();
    } else {
        do {
            classValue = rnd(0, ai.getNumClasses() - 1);
        } while(tGlobals->defaultClassPolicy != DISABLED && classValue == tGlobals->defaultClass);
    }
}

void classifier_hyperrect_list_ratio::mutation()
{
    //    cout << "Mutation" << endl;
    int attIndex;
    
    modif = 1;
    
    if(tGlobals->numClasses > 1 && !rnd < 0.10) {
        int newValue;
        
        do {
            newValue = rnd(0, ai.getNumClasses() - 1);
        } while (newValue == classValue || tGlobals->defaultClassPolicy != DISABLED && newValue == tGlobals->defaultClass);
        
        classValue = newValue;
    } else {
        if(rule->size() > 0) {
            //Use new ratio_test class
            
            attIndex = rnd(0, rule->size() - 1);
            rule->at(attIndex).mutate();
        }
    }
}

/* Print (put to String) */
void classifier_hyperrect_list_ratio::dumpPhenotype(char *string)
{
    //    cout << "Dump Phenotype" << endl;
    
    stringstream output;
    
    for (int i = 0; i < rule->size(); i++) {
        output << rule->at(i).getPhenotype();
    }
    
    output << ai.getNominalValue(tGlobals->numAttributesMC, classValue)->cstr() << "\n";
    
    strcpy(string, output.str().c_str());
}


int classifier_hyperrect_list_ratio::getSpecificity(int *indexes,double *specificity)
{
    //    cout << "Get Specificity" << endl;
    return 1; // Quick fix
}

void classifier_hyperrect_list_ratio::crossover(classifier * in,
                                               classifier * out1, classifier * out2)
{
    //    cout << "Crossover 3 params" << endl;
    
    crossover_1px(this, (classifier_hyperrect_list_ratio *) in,
                  (classifier_hyperrect_list_ratio *) out1,
                  (classifier_hyperrect_list_ratio *) out2);
}

/* Used to recombine lists of expressed attributes */
void classifier_hyperrect_list_ratio::crossover_1px(classifier_hyperrect_list_ratio * in1,
                                                   classifier_hyperrect_list_ratio * in2,
                                                   classifier_hyperrect_list_ratio * out1,
                                                   classifier_hyperrect_list_ratio * out2)
{
    //    cout << "Crossover 4 params" << endl;
    
    vector<ratio_test> *min_rule = in1->rule->size() < in2->rule->size() ? in1->rule : in2->rule;
    vector<ratio_test> *max_rule = in2->rule->size() >= in2->rule->size() ? in1->rule : in2-> rule;
    
    int cutPoint = rnd(0, max_rule->size() - 1);
    
    vector<ratio_test>::const_iterator cut = max_rule->begin() + cutPoint;
    vector<ratio_test>::const_iterator last = max_rule->end();
    vector<ratio_test> max_rule_contrib(cut, last);
    
    max_rule->erase(cut, last);
    
    if (cutPoint <= min_rule->size()) {
        max_rule->insert(max_rule->end(), min_rule->begin() + cutPoint, min_rule->end());
        min_rule->erase(min_rule->begin() + cutPoint, min_rule->end());
    }
    
    min_rule->insert(min_rule->end(), max_rule_contrib.begin(), max_rule_contrib.end());
    
    out1->rule = min_rule;
    out2->rule = max_rule;
}

void classifier_hyperrect_list_ratio::postprocess()
{
    //    cout << "Post Process" << endl;
}

void classifier_hyperrect_list_ratio::doSpecialStage(int stage)
{
    //    cout << "Do Special Stage" << endl;
    
    int i;
    
    if (stage == 0) { //Generalize - remove attributes
        if (rule->size() > 0 && !rnd < tReal->probGeneralizeList) {
            
            int attribute = rnd(0, rule->size() - 1);
            
            rule->erase(rule->begin() + attribute);
        }
    } else { //Specialize - add attributes
        
        // How to specialize when 2 attributes?
        
        if(rule->size() < tGlobals->numAttributesMC && !rnd < tReal->probSpecializeList) {
            
            // Creates a map of attributes, 0 or 1 if present
            int attMap[tGlobals->numAttributesMC];
            bzero(attMap, tGlobals->numAttributesMC * sizeof(int));
            for(i = 0; i < rule->size(); i++) {
                attMap[rule->at(i).attribute1] = 1;
            }
            
            // Find an attribute that isn't expressed
            int selectedAtt;
            do {
                selectedAtt = rnd(0, tGlobals->numAttributesMC - 1);
            } while(attMap[selectedAtt] == 1);
            
            instance *ins = NULL;
            if(tGlobals->smartInit) {
                if(tGlobals->defaultClassPolicy != DISABLED) {
                    ins = is->getInstanceInit(tGlobals->defaultClass);
                } else {
                    ins = is->getInstanceInit(ai.getNumClasses());
                }
            }
            
            
            //            cout << "Size: " << rule->size() << endl;
            
            int loc = ratio_test::binarySearch(*rule, 0, rule->size(), selectedAtt) - 1;
            //            cout << "Pos: " << loc << endl;
            
            rule->insert(rule->begin() + loc, ratio_test(selectedAtt, selectedAtt, ins));
            
        }
    }
}

