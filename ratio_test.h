//
//  ratio_test.h
//  BioHEL
//
//  Created by Euan Cowie on 11/03/2015.
//
//

#ifndef BioHEL_ratio_test_h
#define BioHEL_ratio_test_h

#include "timerRealKR.h"
#include <sstream>

extern timerRealKR *tReal;

class ratio_test {
    
public:
    int attribute1;
    int attribute2;
    float threshold;
    
    inline ~ratio_test() {
        
    }
    
    inline ratio_test(const ratio_test &test) {
        *this = test;
    }
    
    // 2 attributes, 1 fold (value), 1 binary switch (+/-)
    /*
     * When matching, have valus for x and y,
     binary is positive or negate, x/y or y/x
     check if resulting is greater than the fold change (ratio value)
     */
    inline ratio_test(int attIndex1, int attIndex2, instance *ins) : attribute1(attIndex1), attribute2(attIndex2){
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
        
        threshold = min;
    }
    
    inline double computeLength() {
        double length = 0.0;
        
        float size = ai.getSizeDomain(attribute1);
        float maxD = ai.getMaxDomain(attribute1);
        
        if(size > 0) {
            length = 1.0 - (maxD - threshold) / size;
        }
        return length;
    }
    
    inline void mutate() {
        float newValue, minOffset, maxOffset;
        minOffset = maxOffset = 0.5 * ai.getSizeDomain(attribute1);
        newValue = mutationOffset(threshold, minOffset, maxOffset);
        
        if (newValue < ai.getMinDomain(attribute1)) newValue = ai.getMinDomain(attribute1);
        if (newValue > ai.getMaxDomain(attribute1)) newValue = ai.getMaxDomain(attribute1);
        
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
        
        float minD = ai.getMinDomain(attribute1);
        // float maxD = ai.getMaxDomain(attIndex);
        
        stringstream att;
        att << "Att " << ai.getAttributeName(attribute1)->cstr() << " is ";
        
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
        register float value = ins->realValues[attribute1];
        
        if (value >= threshold)
            return true;
        return false;
    }
    
    inline ratio_test& operator=(const ratio_test& test) {
        if (this != &test) {
            attribute1 = test.attribute1;
            threshold = test.threshold;
        }
        return *this;
    }
    
    inline static int binarySearch(std::vector<ratio_test> rule, int from, int to, int key) {
        int low = from;
        int high = to - 1;
        
        while (low <= high) {
            int mid = (low + high) >> 1;
            int midVal = rule[mid].attribute1;
            
            if (midVal < key)
                low = mid + 1;
            else if (midVal > key)
                high = mid - 1;
            else
                return mid; // key found
        }
        return low + 1;  // key not found.
    }
    
    inline bool operator>(ratio_test const& rhs) {
        return attribute1 > rhs.attribute1;
    }
    
    inline bool operator<(ratio_test const& rhs) {
        return attribute1 < rhs.attribute1;
    }
    
};

#endif
