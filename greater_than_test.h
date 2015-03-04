//
//  greater_than_test.h
//  BioHEL
//
//  Created by Euan Cowie on 03/03/2015.
//
//

class greater_than_test {
    
public:
    int attribute;
    float threshold;
    
    inline greater_than_test() {
        
    }
    
    inline ~greater_than_test() {
        
    }
    
    inline greater_than_test(int attIndex, float lower) {
        attribute = attIndex;
        threshold = lower;
    }
    
    inline bool isTrue(float value) {
        if (value >= threshold)
            return true;
        return false;
    }
    
};