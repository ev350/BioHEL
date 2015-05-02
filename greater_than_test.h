//
//  greater_than_test.h
//  BioHEL
//
//  Created by Euan Cowie on 03/03/2015.
//
//

#ifndef _GREATER_THAN_TEST_
#define _GREATER_THAN_TEST_

#include "test.h"
#include <sstream>

class greater_than_test : public test {
    
public:
    
    greater_than_test(const greater_than_test &test) {
        *this = test;
    }
    
    greater_than_test(instance *ins) {
        attribute = tReal->sampAtts->getSample();
        
        float max, min;
        float sizeD = ai.getSizeDomain(attribute);
        float maxD = ai.getMaxDomain(attribute);
        float size = (!rnd * tReal->rangeIntervalSizeInit + tReal->minIntervalSizeInit) * sizeD;
        
        max = maxD;
        min = maxD - size;
        
        if(ins) {
            float value = ins->realValues[attribute];
            if(value < min) {
                min = value;
            }
        }
        
        threshold = min;
    }
    
    virtual double computeLength() const {
        double length = 0.0;
        
        float size = ai.getSizeDomain(attribute);
        float maxD = ai.getMaxDomain(attribute);
        
        if(size > 0) {
            length = 1.0 - (maxD - threshold) / size;
        }
        
        return length;
    }
    
    virtual string getPhenotype() const {
        
        float minD = ai.getMinDomain(attribute);
        // float maxD = ai.getMaxDomain(attIndex);
        
        stringstream att;
        att << "Att " << ai.getAttributeName(attribute)->cstr() << " is ";
        
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
        register float value = ins->realValues[attribute];
        
        if (value < threshold)
            return true;
        return false;
    }
    
    virtual greater_than_test& operator=(const greater_than_test& test) {
        if (this != &test) {
            attribute = test.attribute;
            threshold = test.threshold;
        }
        return *this;
    }
    
};

#endif