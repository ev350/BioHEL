//
//  ratio_test.h
//  BioHEL
//
//  Created by Euan Cowie on 11/03/2015.
//
//

#ifndef BioHEL_ratio_test_h
#define BioHEL_ratio_test_h

#include "test.h"
#include <sstream>

extern timerRealKR *tReal;

class ratio_test : public test {
    
public:
    int attribute1;
    
    inline ratio_test(const ratio_test &test) {
        *this = test;
    }
    
    // 2 attributes, 1 fold (value), 1 binary switch (+/-)
    /*
     * When matching, have valus for x and y,
     binary is positive or negate, x/y or y/x
     check if resulting is greater than the fold change (ratio value)
     */
    inline ratio_test(int bSwitch, instance *ins) {
        attribute = tReal->sampAtts->getSample();
        attribute1 = tReal->sampAtts->getSample();
        
        // if true then x/y -> y/x
        if (bSwitch == 1)
            swap(attribute, attribute1);
        
        float max, min;
        float sizeD = ai.getSizeDomain(attribute1);
        float maxD = ai.getMaxDomain(attribute1);
        float size = (!rnd * tReal->rangeIntervalSizeInit + tReal->minIntervalSizeInit) * sizeD;
        
        max = maxD;
        min = maxD - size;
        
        if(ins) {
            float value = ins->realValues[attribute1];
            if(value < min) {
                min = value;
            }
        }
        
        threshold = min; // How should threshold be calculated?
    }
    
    
    virtual string getPhenotype() const {
        
        float minD = ai.getMinDomain(attribute1);
        // float maxD = ai.getMaxDomain(attIndex);
        
        stringstream att;
        att << "Att " << ai.getAttributeName(attribute)->cstr() << " / " << "Att " << ai.getAttributeName(attribute1)->cstr() << " is ";
        
        bool irr = false;
        if (threshold == minD) {
            // Do nothing
            irr = true;
        } else {
            att << "[>" << threshold << "]" << "|";
        }
        
        if (!irr)
            return att.str();
        else
            return "";
    }
    
    virtual bool isTrue(instance *ins) const {
        register float value = ins->realValues[attribute1];
        
        if (value >= threshold)
            return true;
        return false;
    }
    
    virtual ratio_test& operator=(const ratio_test& test) {
        if (this != &test) {
            attribute = test.attribute;
            attribute1 = test.attribute1;
            threshold = test.threshold;
        }
        return *this;
    }
    
};

#endif
