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
    int bSwitch;
    
    float sizeD;
    
    inline ratio_test(const ratio_test &test) {
        *this = test;
    }
    
    // 2 attributes, 1 fold (value), 1 binary switch (+/-)
    /*
     * When matching, have valus for x and y,
     binary is positive or negate, x/y or y/x
     check if resulting is greater than the fold change (ratio value)
     */
    inline ratio_test(instance *ins) {
        attribute = tReal->sampAtts->getSample();
        attribute1 = tReal->sampAtts->getSample();
        bSwitch = rnd(0, 1);
        
        // if true then x/y -> y/x
        if (bSwitch == 1)
            swap(attribute, attribute1);
        
        sizeD = ai.getSizeDomain(attribute1) / ai.getSizeDomain(attribute);
        float maxD = tReal->max_ratio;
        threshold = !rnd * maxD;
        
        if(ins) {
            float value = ins->realValues[attribute] / ins->realValues[attribute1];
            if(value * sizeD < threshold) {
                threshold = value * sizeD;
            }
        }
        
    }
    
    virtual double computeLength() const {
        return 1.0 - threshold / tReal->max_ratio;
    }
    
    virtual void mutate() {
        if (rnd(0,1)) {
            bSwitch = bSwitch ? 0 : 1;
            swap(attribute, attribute1);
            sizeD = ai.getSizeDomain(attribute1) / ai.getSizeDomain(attribute);
        } else {
            float newValue, minOffset, maxOffset;
            
            minOffset = maxOffset = 0.5 * ai.getSizeDomain(attribute);
            newValue = mutationOffset(threshold, minOffset, maxOffset);
            
            if (newValue < 0) newValue = 0;
            if (newValue > tReal->max_ratio) newValue = tReal->max_ratio;
            
            threshold = newValue;
        }
    }
    
    virtual string getPhenotype() const {        
        stringstream att;
        
        att << "Att " << ai.getAttributeName(attribute)->cstr() << " / " << "Att " << ai.getAttributeName(attribute1)->cstr() << " sizeD:" << sizeD << " is ";
    
        
        att << "[>" << threshold << "]" << "|";
        
        return att.str();
    }
    
    virtual bool isTrue(instance *ins) const {
        register float value = ins->realValues[attribute] / ins->realValues[attribute1];
        
        //        std::cout << ai.getAttributeName(attribute)->cstr() << " / " << ai.getAttributeName(attribute1)->cstr() << " - attsValue: " << value << std::endl;
        
        if ((value * sizeD) > threshold)
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
