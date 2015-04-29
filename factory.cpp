#include "factory.h"

#include "test.h"
#include "classifier_gabil.h"
//#include "classifier_lcs.h"
//#include "classifier_adaptive.h"
#include "classifier_hyper_list.h"
#include "classifier_hyperrect.h"
#include "classifier_hyperrect_list.h"
#include "classifier_hyperrect_list_real.h"
#include "classifier_hyperrect_list_ratio.h"
//#include "classifier_hyperrect_sse.h"
//#include "classifier_rotated_hyperrect.h"
//#include "classifier_instances.h"
#include "configManagement.h"
#include "instanceSet.h"
#include "attributesInfo.h"
#include "timerGlobals.h"

extern configManagement cm;
extern instanceSet *is;
extern attributesInfo ai;
extern timerGlobals *tGlobals;

classifierFactory::classifierFactory()
{
    
    if (cm.thereIsParameter(KR_HYPERRECT)) {
        if(cm.thereIsParameter(HYPERRECT_LIST)) {
            if(ai.onlyRealValuedAttributes()) {
                if(cm.thereIsParameter(USE_GREATER_THAN_PREDICATES)) {
                    classifierType = KR_HYPERRECT_LIST_PLUS;
                } else if(cm.thereIsParameter(USE_LESS_THAN_PREDICATES)) {
                    classifierType = KR_HYPERRECT_LIST_LESS;
                } else if(cm.thereIsParameter(USE_RATIO_PREDICATES)) {
                    classifierType = KR_HYPERRECT_LIST_RATIO;
                } else {
                    classifierType = KR_HYPERRECT_LIST_REAL;
                }
            } else {
                classifierType = KR_HYPERRECT_LIST;
            }
        } else {
            classifierType = KR_HYPERRECT;
        }
    } else {
        classifierType = KR_GABIL;
    }
}


classifier *classifierFactory::createClassifier()
{
    if (classifierType == KR_HYPERRECT)
        return new classifier_hyperrect();
    if (classifierType == KR_HYPERRECT_LIST)
        return new classifier_hyperrect_list();
    if (classifierType == KR_HYPERRECT_LIST_REAL)
        return new classifier_hyper_list(test::Bounds);
    if (classifierType == KR_HYPERRECT_LIST_PLUS)
        return new classifier_hyper_list(test::Greater);
    if (classifierType == KR_HYPERRECT_LIST_LESS)
        return new classifier_hyper_list(test::Less);
    if (classifierType == KR_HYPERRECT_LIST_RATIO)
        return new classifier_hyper_list(test::Ratio);
    
    return new classifier_gabil();
}


classifier *classifierFactory::cloneClassifier(classifier * orig, int son)
{
    return new classifier_hyper_list(*((classifier_hyper_list *) orig), son);
}

void classifierFactory::deleteClassifier(classifier * ind)
{
    delete (classifier_hyper_list *)ind;
}
