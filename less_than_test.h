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
    
    virtual double computeLength() const {
        double length = 0.0;
        
        float size = ai.getSizeDomain(attribute);
        float minD = ai.getMinDomain(attribute);
        
        if(size > 0) {
            length += 1.0 - (threshold - minD) / size;
        }
        
        return length;
    }
    
    virtual void mutate() {
        float newValue, minOffset, maxOffset;
        
        minOffset = maxOffset = 0.5 * ai.getSizeDomain(attribute);
        newValue = mutationOffset(threshold, minOffset, maxOffset);
        
        if (newValue < ai.getMinDomain(attribute)) newValue = ai.getMinDomain(attribute);
        if (newValue > ai.getMaxDomain(attribute)) newValue = ai.getMaxDomain(attribute);
        
        threshold = newValue;
    }
    
    virtual string getPhenotype() const {
        stringstream att;
        att << "Att " << ai.getAttributeName(attribute)->cstr() << " is ";
        
        att << "[<" << threshold << "]" << "|";
        
        return att.str();
        
    }
    
    virtual bool isTrue(instance *ins) const {
        register float value = ins->realValues[attribute];
        
        if (value < threshold)
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