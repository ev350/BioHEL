//
//  bounds_test.h
//  BioHEL
//
//  Created by Euan Cowie on 23/04/2015.
//
//

#ifndef BioHEL_bounds_test_h
#define BioHEL_bounds_test_h

#include "test.h"
#include <sstream>

class bounds_test : public test {
    
    float threshold1;
    
public:
    
    bounds_test(const greater_than_test &test) {
        *this = test;
    }
    
    bounds_test(instance *ins) {
        attribute = tReal->sampAtts->getSample();
        
        float max, min;
        float sizeD = ai.getSizeDomain(attribute);
        float minD = ai.getMinDomain(attribute);
        float maxD = ai.getMaxDomain(attribute);
        float size = (!rnd * tReal->rangeIntervalSizeInit + tReal->minIntervalSizeInit) * sizeD;
        
        if(ins) {
            float val = ins->realValues[attribute];
            min = val-size/2.0;
            max = val+size/2.0;
            if(min < minD) {
                max += minD - min;
                min = minD;
            }
            if(max > maxD) {
                min -= max - maxD;
                max = maxD;
            }
        } else {
            min=!rnd*(sizeD-size)+minD;
            max=min+size;
        }
        
        threshold = min;
        threshold1 = max;
    }
    
    virtual double computeLength() const {
        double length = 0.0;
        
        float size = ai.getSizeDomain(attribute);
        
        if(size > 0) {
            length += 1.0 - (threshold1 - threshold) / size;
        }
        
        return length;
    }
    
    virtual string getPhenotype() const {
        
        float minD = ai.getMinDomain(attribute);
//        float maxD = ai.getMaxDomain(attribute);
        
        stringstream att;
        att << "Att " << ai.getAttributeName(attribute)->cstr() << " is ";
        
        bool irr = true;
        if (threshold == minD) {
            
        } else {
            irr = false;
            att << "[" << threshold << "," << threshold1 << "]" << "|";
        }
        
        if (!irr)
            return att.str();
        else
            return "";
    }
    
    virtual bool isTrue(instance *ins) const {
        register float value = ins->realValues[attribute];
        
        if (value < threshold || value > threshold1)
            return true;
        return false;
    }
    
    virtual bounds_test& operator=(const bounds_test& test) {
        if (this != &test) {
            attribute = test.attribute;
            threshold = test.threshold;
            threshold1 = test.threshold1;
        }
        return *this;
    }
    
};

#endif
