//
//  greater_than_test.h
//  BioHEL
//
//  Created by Euan Cowie on 03/03/2015.
//
//

#ifndef _GREATER_THAN_TEST_
#define _GREATER_THAN_TEST_

#include "timerRealKR.h"
#include <sstream>

extern timerRealKR *tReal;

class greater_than_test {
    
public:
    int attribute;
    float threshold;
    
    inline ~greater_than_test() {
        
    }
    
    inline greater_than_test(const greater_than_test &test) {
        *this = test;
    }
    
    inline greater_than_test(int attIndex, instance *ins) : attribute(attIndex){
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
    
    inline double computeLength() {
        double length = 0.0;
        
        float size = ai.getSizeDomain(attribute);
        float maxD = ai.getMaxDomain(attribute);
        
        if(size > 0) {
            length = 1.0 - (maxD - threshold) / size;
        }
        return length;
    }
    
    inline void mutate() {
        float newValue, minOffset, maxOffset;
        minOffset = maxOffset = 0.5 * ai.getSizeDomain(attribute);
        newValue = mutationOffset(threshold, minOffset, maxOffset);
        
        if (newValue < ai.getMinDomain(attribute)) newValue = ai.getMinDomain(attribute);
        if (newValue > ai.getMaxDomain(attribute)) newValue = ai.getMaxDomain(attribute);
        
        threshold = newValue;
    }
    
    inline float mutationOffset(float geneValue, float offsetMin, float offsetMax) {
        float newValue;
        if (!rnd < 0.5) {
            newValue = geneValue + !rnd * offsetMax;
        } else {
            newValue = geneValue - !rnd * offsetMin;
        }
        return newValue;
    }
    
    inline string getPhenotype() {
        
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
    
    inline bool isTrue(instance *ins) {
        register float value = ins->realValues[attribute];
        
        if (value >= threshold)
            return true;
        return false;
    }
    
    inline greater_than_test& operator=(const greater_than_test& test) {
        if (this != &test) {
            attribute = test.attribute;
            threshold = test.threshold;
        }
        return *this;
    }
    
    inline static int binarySearch(std::vector<greater_than_test> rule, int from, int to, int key) {
        int low = from;
        int high = to - 1;
        
        while (low <= high) {
            int mid = (low + high) >> 1;
            int midVal = rule[mid].attribute;
            
            if (midVal < key)
                low = mid + 1;
            else if (midVal > key)
                high = mid - 1;
            else
                return mid; // key found
        }
        return low + 1;  // key not found.
    }
    
    inline bool operator>(greater_than_test const& rhs) {
        return attribute > rhs.attribute;
    }
    
    inline bool operator<(greater_than_test const& rhs) {
        return attribute < rhs.attribute;
    }
    
};

#endif