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
        Ratio,
        Mixed
    };
    
    int attribute;
    float threshold;
    
    virtual double computeLength() const = 0;
    
    virtual void mutate() = 0;
    
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