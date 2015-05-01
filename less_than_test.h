//
//  less_than_test.h
//  BioHEL
//
//  Created by Euan Cowie on 03/03/2015.
//
//

#ifndef _LESS_THAN_TEST_
#define _LESS_THAN_TEST_

#include "test.h"
#include <sstream>

class less_than_test : public test {
    
public:
    
    less_than_test(const less_than_test &test) {
        *this = test;
    }
    
    less_than_test(instance *ins) {
        attribute = tReal->sampAtts->getSample();
        
        float max, min;
        float sizeD = ai.getSizeDomain(attribute);
        float minD = ai.getMinDomain(attribute);
        float size = (!rnd * tReal->rangeIntervalSizeInit + tReal->minIntervalSizeInit) * sizeD;
        
        min = minD;
        max = min+size;
        
        if(ins) {
            float value = ins->realValues[attribute];
            if(value > max) {
                max = value;
            }
        }
        
        threshold = max;
    }
    
    virtual string getPhenotype() const {
        
//        float minD = ai.getMinDomain(attribute);
         float maxD = ai.getMaxDomain(attribute);
        
        stringstream att;
        att << "Att " << ai.getAttributeName(attribute)->cstr() << " is ";
        
        bool irr = false;
        if (threshold == maxD) {
            // Do nothing
            irr = true;
        } else {
            att << "[<" << threshold << "]" << "|";
        }
        
        if (!irr)
            return att.str();
        else
            return "";
    }
    
    virtual bool isTrue(instance *ins) const {
        register float value = ins->realValues[attribute];
        
        if (value > threshold)
            return true;
        return false;
    }
    
    virtual less_than_test& operator=(const less_than_test& test) {
        if (this != &test) {
            attribute = test.attribute;
            threshold = test.threshold;
        }
        return *this;
    }
    
};

#endif