//
//  test.h
//  BioHEL
//
//  Created by Euan Cowie on 23/04/2015.
//
//

#ifndef _TEST_
#define _TEST_

#include "classifier.h"
#include "timerRealKR.h"

extern timerRealKR *tReal;

class test {
    
public:
    enum Type {
        Bounds,
        Greater,
        Less,
        Ratio
    };
    
    int attribute;
    float threshold;
    
    double computeLength() {
        double length = 0.0;
        
        float size = ai.getSizeDomain(attribute);
        float maxD = ai.getMaxDomain(attribute);
        
        if(size > 0) {
            length = 1.0 - (maxD - threshold) / size;
        }
        return length;
    }
    
    void mutate() {
        float newValue, minOffset, maxOffset;
        minOffset = maxOffset = 0.5 * ai.getSizeDomain(attribute);
        newValue = mutationOffset(threshold, minOffset, maxOffset);
        
        if (newValue < ai.getMinDomain(attribute)) newValue = ai.getMinDomain(attribute);
        if (newValue > ai.getMaxDomain(attribute)) newValue = ai.getMaxDomain(attribute);
        
        threshold = newValue;
    }
    
    float mutationOffset(float geneValue, float offsetMin, float offsetMax) {
        float newValue;
        if (!rnd < 0.5) {
            newValue = geneValue + !rnd * offsetMax;
        } else {
            newValue = geneValue - !rnd * offsetMin;
        }
        return newValue;
    }
    
    virtual string getPhenotype() const = 0;
    virtual bool isTrue(instance *ins) const = 0;
    
    bool operator>(test const& rhs) {
        return attribute > rhs.attribute;
    }
    
    bool operator<(test const& rhs) {
        return attribute < rhs.attribute;
    }
    
};

#endif