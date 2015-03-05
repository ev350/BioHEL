#include "classifier_hyperrect_list_plus.h"
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

double classifier_hyperrect_list_plus::computeTheoryLength()
{
    int i;
    theoryLength = 0.0;
    
    for(i = 0; i < rule.size(); i++) {
        // Using new greater_than_test class
        theoryLength += rule[i].computeLength();
    }
    
    theoryLength /= (double)tGlobals->numAttributesMC;
    
    return theoryLength;
}

classifier_hyperrect_list_plus::classifier_hyperrect_list_plus()
{
    classifier_hyperrect_list_plus::initializeChromosome();
}

classifier_hyperrect_list_plus::classifier_hyperrect_list_plus(const classifier_hyperrect_list_plus & orig, int son)
{
    *this = orig;
    
    if (!son) {
        rule = orig.rule;
    }
}

classifier_hyperrect_list_plus::~classifier_hyperrect_list_plus()
{
    
}

void classifier_hyperrect_list_plus::initializeChromosome()
{
    std::cout << "Initialize Chromosome" << std::endl;
    
    int i;
    int numAtt;
    
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
    
    numAtt = selectedAtts.size();
    
    for(i = 0; i < numAtt; i++) {
        rule.addElement(greater_than_test(selectedAtts[i], ins));
    }
    
    if(ins) {
        classValue = ins->getClass();
    } else {
        do {
            classValue = rnd(0, ai.getNumClasses() - 1);
        } while(tGlobals->defaultClassPolicy != DISABLED && classValue == tGlobals->defaultClass);
    }
}

void classifier_hyperrect_list_plus::mutation()
{
    std::cout << "Mutation" << std::endl;
    int attIndex;
    
    modif = 1;
    
    if(tGlobals->numClasses > 1 && !rnd < 0.10) {
        int newValue;
        
        do {
            newValue = rnd(0, ai.getNumClasses() - 1);
        } while (newValue == classValue || tGlobals->defaultClassPolicy != DISABLED && newValue == tGlobals->defaultClass);
        
        classValue = newValue;
    } else {
        if(rule.size() > 0) {
            //Use new greater_than_test class
            
            attIndex = rnd(0, rule.size() - 1);
            rule[attIndex].mutate();
        }
    }
}

/* Print (put to String) */
void classifier_hyperrect_list_plus::dumpPhenotype(char *string)
{
    std::cout << "Dump Phenotype" << std::endl;
    
    stringstream output;
    
    int index = 0;
    
    for (int i = 0; i < rule.size(); i++) {
        
        int attIndex = rule[i].attribute; // Used to get the attribute minD, maxD and name
        
        float minD = ai.getMinDomain(attIndex);
        //        float maxD = ai.getMaxDomain(attIndex);
        
        stringstream att;
        att << "Att " << ai.getAttributeName(attIndex)->cstr() << " is ";
        
        bool irr = false;
        
        if (rule[i].threshold == minD) {
            // Do nothing
        } else {
            att << "[>" << rule[i].threshold << "]" << "|";
        }
        
        if(!irr)
            output << att.str();
        
        index += tReal->attributeSize[attIndex];
    }
    
    output << ai.getNominalValue(tGlobals->numAttributesMC, classValue)->cstr() << "\n";
    
    strcpy(string, output.str().c_str());
}


int classifier_hyperrect_list_plus::getSpecificity(int *indexes,double *specificity)
{
    std::cout << "Get Specificity" << std::endl;
    return 1; // Quick fix
}

void classifier_hyperrect_list_plus::crossover(classifier * in,
                                               classifier * out1, classifier * out2)
{
    std::cout << "Crossover 3 params" << std::endl;
    
    crossover_1px(this, (classifier_hyperrect_list_plus *) in,
                  (classifier_hyperrect_list_plus *) out1,
                  (classifier_hyperrect_list_plus *) out2);
}

/* Used to recombine lists of expressed attributes */
void classifier_hyperrect_list_plus::crossover_1px(classifier_hyperrect_list_plus * in1,
                                                   classifier_hyperrect_list_plus * in2,
                                                   classifier_hyperrect_list_plus * out1,
                                                   classifier_hyperrect_list_plus * out2)
{
    std::cout << "Crossover 4 params" << std::endl;
    
    int i;
    
    out1->modif = out2->modif = 1; // Not sure if necessary
    
    // Making sure the first parent is not empty?
    if(in1->rule.size() == 0) {
        classifier_hyperrect_list_plus *tmp = in2;
        in2 = in1;
        in1 = tmp;
    }
    
    // Probably not necessary
    if(in1->rule.size() == 0) {
        out1->rule.removeAllElements();
        out2->rule.removeAllElements();
        return;
    }
    
    int cutPoint1 = rnd(0, in1->rule.size() - 1);
    //    int selAtt1 = in1->rule.at(cutPoint1)->attribute;
    
    int cutPoint2 = rnd(0, in2->rule.size() - 1);
    //    int selAtt2 = in2->rule.at(cutPoint2)->attribute;
    
    for (i = 0; i < cutPoint2; ++i) {
        out2->rule.addElement(in2->rule[i]);
    }
    
    for (i = cutPoint1; i < in1->rule.size(); ++i) {
        out2->rule.addElement(in1->rule[i]);
    }
    
    for (i = 0; i < cutPoint1; ++i) {
        out1->rule.addElement(in1->rule[i]);
    }
    
    for (i = cutPoint2; i < in2->rule.size(); ++i) {
        out1->rule.addElement(in2->rule[i]);
    }
    
    // Need to check if attribute in both chromosomes
    
    if(!rnd < 0.5) {
        out1->classValue = in1->classValue;
        out2->classValue = in2->classValue;
    } else {
        out1->classValue = in2->classValue;
        out2->classValue = in1->classValue;
    }
}

void classifier_hyperrect_list_plus::postprocess()
{
    std::cout << "Post Process" << std::endl;
}

void classifier_hyperrect_list_plus::doSpecialStage(int stage)
{
    std::cout << "Do Special Stage" << std::endl;
    
    int i;
    
    if (stage == 0) { //Generalize - remove attributes
        if (rule.size() > 0 && !rnd < tReal->probGeneralizeList) {
            
            int attribute = rnd(0, rule.size() - 1);
            
            rule.removeElementAt(attribute);
        }
    } else { //Specialize - add attributes
        
        if(rule.size() < tGlobals->numAttributesMC && !rnd < tReal->probSpecializeList) {
            
            // Creates a map of attributes, 0 or 1 if present
            int attMap[tGlobals->numAttributesMC];
            bzero(attMap, tGlobals->numAttributesMC * sizeof(int));
            for(i = 0; i < rule.size(); i++) {
                attMap[rule[i].attribute] = 1;
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
            
            for (int i = 0; i < rule.size(); ++i) {
                std::cout << rule[i].attribute << std::endl;
            }
            
            int loc = greater_than_test::binarySearch(rule, 0, rule.size(), selectedAtt);
            
            std::cout << loc << std::endl;
            
            rule.insertElementAt(greater_than_test(selectedAtt, ins), loc);
            
        }
    }
}

