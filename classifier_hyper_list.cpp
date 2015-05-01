//
//  classifier_hyper_list.cpp
//  BioHEL
//
//  Created by Euan Cowie on 23/04/2015.
//

#include "classifier_hyper_list.h"
#include "testFactory.h"
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

double classifier_hyper_list::computeTheoryLength()
{
    int i;
    theoryLength = 0.0;
    
    for(i = 0; i < rule->size(); i++) {
        theoryLength += rule->at(i)->computeLength();
    }
    
    theoryLength /= (double)tGlobals->numAttributesMC;
    
    return theoryLength;
}

classifier_hyper_list::classifier_hyper_list(test::Type type) : type(type)
{
    classifier_hyper_list::initializeChromosome();
}

classifier_hyper_list::classifier_hyper_list(const classifier_hyper_list & orig, int son)
{
    *this = orig;
    
    if (!son) {
        rule = new vector<test*>(*orig.rule);
    }
}

classifier_hyper_list::~classifier_hyper_list()
{
    
}

// Refactor
void classifier_hyper_list::initializeChromosome()
{
    rule = new vector<test*>();
    int i;
    
    instance *ins = NULL;
    if(tGlobals->smartInit) {
        if(tGlobals->defaultClassPolicy != DISABLED) {
            ins = is->getInstanceInit(tGlobals->defaultClass);
        } else {
            ins = is->getInstanceInit(ai.getNumClasses());
        }
    }
    
    if (tReal->fixExpAtt) {
        for (i = 0; i < tReal->numExpAtt; i++) {
            auto newTest = testFactory::createInstance(type, ins);
            rule->push_back(newTest);
        }
        sort(rule->begin(), rule->end());
    } else {
        for (i = 0; i < tGlobals->numAttributesMC; i++) {
            if(!rnd >= tReal->probIrr) {
                auto newTest = testFactory::createInstance(type, ins);
                rule->push_back(newTest);
            }
        }
    }
    
    if(ins) {
        classValue = ins->getClass();
    } else {
        do {
            classValue = rnd(0, ai.getNumClasses() - 1);
        } while(tGlobals->defaultClassPolicy != DISABLED && classValue == tGlobals->defaultClass);
    }
}

void classifier_hyper_list::mutation()
{
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
            //Use new test class
            
            attIndex = rnd(0, rule->size() - 1);
            rule->at(attIndex)->mutate();
        }
    }
}

void classifier_hyper_list::dumpPhenotype(char *string)
{
    stringstream output;
    
    for (int i = 0; i < rule->size(); i++) {
        output << rule->at(i)->getPhenotype();
    }
    
    output << ai.getNominalValue(tGlobals->numAttributesMC, classValue)->cstr() << "\n";
    
    strcpy(string, output.str().c_str());
}


int classifier_hyper_list::getSpecificity(int *indexes,double *specificity)
{
    return 1; // Quick fix
}

void classifier_hyper_list::crossover(classifier * in,
                                      classifier * out1, classifier * out2)
{
    crossover_1px(this, (classifier_hyper_list *) in,
                  (classifier_hyper_list *) out1,
                  (classifier_hyper_list *) out2);
}

void classifier_hyper_list::crossover_1px(classifier_hyper_list * in1,
                                          classifier_hyper_list * in2,
                                          classifier_hyper_list * out1,
                                          classifier_hyper_list * out2)
{
    vector<test*> *min_rule = in1->rule->size() < in2->rule->size() ? in1->rule : in2->rule;
    vector<test*> *max_rule = in2->rule->size() >= in2->rule->size() ? in1->rule : in2-> rule;
    
    int cutPoint = rnd(0, max_rule->size() - 1);
    
    vector<test*>::const_iterator cut = max_rule->begin() + cutPoint;
    vector<test*>::const_iterator last = max_rule->end();
    vector<test*> max_rule_contrib(cut, last);
    
    max_rule->erase(cut, last);
    
    if (cutPoint <= min_rule->size()) {
        max_rule->insert(max_rule->end(), min_rule->begin() + cutPoint, min_rule->end());
        min_rule->erase(min_rule->begin() + cutPoint, min_rule->end());
    }
    
    min_rule->insert(min_rule->end(), max_rule_contrib.begin(), max_rule_contrib.end());
    
    out1->rule = min_rule;
    out2->rule = max_rule;
}

void classifier_hyper_list::postprocess()
{
    //    cout << "Post Process" << endl;
}

void classifier_hyper_list::doSpecialStage(int stage)
{
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
                attMap[rule->at(i)->attribute] = 1;
            }
            
            // create a function "presence?", pass array as argument then create bitmap
            
            // will need a factory later on to chose which one of the variants
            
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
            
            
            int loc = classifier_hyper_list::binarySearch(*rule, 0, rule->size(), selectedAtt) - 1;
            
            auto newTest = testFactory::createInstance(type, ins);
            
            rule->insert(rule->begin() + loc, newTest); //Select type
            
        }
    }
}
