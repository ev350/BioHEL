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

#include "greater_than_test.h"
#include "less_than_test.h"
#include "ratio_test.h"
#include "bounds_test.h"

class testFactory {
    
public:
    static test* createInstance(test::Type type, int att, instance *ins) {
        switch (type) {
            case test::Greater:
                return new greater_than_test(att, ins);
                
            case test::Less:
                return new less_than_test(att, ins);
                
            case test::Bounds:
                return new bounds_test(att, ins);
                
            default:
                return nullptr;
        }
    }
};

#endif
