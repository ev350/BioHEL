#ifndef _CLASSIFIER_FACTORY_H_
#define _CLASSIFIER_FACTORY_H_

#include "classifier.h"

class classifierFactory {
	int classifierType;
public:
	
	classifierFactory();
	classifier *createClassifier();
	classifier *cloneClassifier(classifier *orig,int son=0);
	void deleteClassifier(classifier *ind);
};

#endif
