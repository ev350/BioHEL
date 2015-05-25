//
//  classifierFactory.h
//  BioHEL
//
//  Created by Euan Cowie on 23/04/2015.
//
//

#ifndef BioHEL_classifierFactory_h
#define BioHEL_classifierFactory_h

#include <memory>


#include "random.h"
#include "greater_than_test.h"
#include "less_than_test.h"
#include "ratio_test.h"
#include "bounds_test.h"

extern Random rnd;

class testFactory {
    
public:
    static test* createInstance(test::Type type, instance *ins) {
        switch (type) {
            case test::Greater:
                return new greater_than_test(ins);
                
            case test::Less:
                return new less_than_test(ins);
                
            case test::Bounds:
                return new bounds_test(ins);
                
            case test::Ratio:
                return new ratio_test(ins);
                
            case test::Mixed:
                return createRandomInstance(ins);
        }
        
        return NULL;
    }
    
private:
    static test* createRandomInstance(instance *ins) {
        
        int testNum = rnd(0, 3);
        
        switch (testNum) {
            case 0:
                return new greater_than_test(ins);
                
            case 1:
                return new less_than_test(ins);
                
            case 2:
                return new bounds_test(ins);
                
            case 3:
                return new ratio_test(ins);
                
        }
        return NULL;
    }
};

#endif
