//
//  classifierFactory.cpp
//  BioHEL
//
//  Created by Euan Cowie on 23/04/2015.
//
//

#include "classifierFactory.h"

test* testFactory::createInstance(test::Type type, int att, instance *ins) {
    test* instance = nullptr;
    
    switch (type) {
        case test::Greater:
            instance = new greater_than_test(att, ins);
            break;
            
        case test::Less:
            instance = new less_than_test(att, ins);
            break;
            
        default:
            break;
    }
    
    if(instance != nullptr)
        return instance;
    else
        return nullptr;
}