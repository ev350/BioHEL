#ifndef _ATTRIBUTES_INFO_H_
#define _ATTRIBUTES_INFO_H_

#include "JString.h"
#include "JVector.h"
#include "instance.h"

#define NOMINAL 1
#define ENTER 2
#define REAL 3

class attributesInfo {
	int numAttributes;
	int numAttributesMC;
	int *typeOfAttributes;
	int *nominalAttributes;
	int *realAttributes;
	int numNominal;
	int numReal;
	float *minDomain,*maxDomain,*sizeDomain,*sizeDomain2;
	float *averageAttribute;
	int *countValues;
	int *classOfInstances;
	int *mostFrequentValueAttribute;
	int **countValuesAttribute;
	JVector<JString *> *valuesOfNominalAttributes;
	JVector<JString *> attributeNames;
	int numExamples;
	int mostFrequentClass;
	int leastFrequentClass;
	int thereAreNominal;
	int thereAreRealValued;

public:
	attributesInfo();
	void setNumAttributes(int num);
	inline int getNumAttributes() {return numAttributes;}
	inline int getNumAttributesMC() {return numAttributesMC;}
	void setTypeOfAttribute(int attribute,int type);
	void insertNominalValue(int attribute,JString *value);
	JString *getNominalValue(int attribute,int value);
	
	void insertAttributeName(JString *name) {
		attributeNames.addElement(name);
	}

	JString *getAttributeName(int attr) {
		return attributeNames.elementAt(attr);
	}


	int getNumValuesAttribute(int attribute);
	int getInstancesOfClass(int pClass) {return classOfInstances[pClass];}
	int thereAreNominalAttributes() {return thereAreNominal;}
	int thereAreRealValuedAttributes() {return thereAreRealValued;}
	int onlyRealValuedAttributes() {
		return (thereAreRealValued && !thereAreNominal);
	}
	int onlyNominalAttributes() {
		return (!thereAreRealValued && thereAreNominal);
	}

	inline int getMostFrequentClass() {return mostFrequentClass;}
	inline int getLeastFrequentClass() {return leastFrequentClass;}
	
	inline int getNumClasses() 
	{
		return valuesOfNominalAttributes[numAttributes-1].size();
	}

	void insertInstance(instance *i);

	inline int getTypeOfAttribute(int attribute)
	{
	#ifdef DEBUG
		if(attribute<0 || attribute>=numAttributes ) {
			fprintf(stderr,"Incorrect values at attributesInfo::getTypeOfAttribute %d\n",attribute);
			exit(1);
		}
	#endif
		return typeOfAttributes[attribute];
	}


	void calculateAverages();
	float getAverageOfAttribute(int attribute);
	int getMostFrequentValueOfAttribute(int attribute);
	void setBounds(float *min,float *max);
	float getMinDomain(int attribute) {return minDomain[attribute];}
	float getMaxDomain(int attribute) {return maxDomain[attribute];}
	float getSizeDomain(int attribute) {return sizeDomain[attribute];}
	float getSizeDomain2(int attribute) {return sizeDomain2[attribute];}

	int valueOfNominalAttribute(int attribute,char *def);

	inline int getNumNominalAttributes() { return numNominal;}
	inline int getNumRealAttributes() { return numReal;}
	inline int *getRealAttributes() { return realAttributes;}
	inline int *getNominalAttributes() { return nominalAttributes;}
};

#endif
