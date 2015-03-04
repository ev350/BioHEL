#include "attributesInfo.h"
#include <stdio.h>
#include "messageBuffer.h"
#include "configManagement.h"
#include <math.h>

extern messageBuffer mb;
extern configManagement cm;

attributesInfo::attributesInfo()
{
	numAttributes=-1;
	numExamples=0;
	thereAreNominal=thereAreRealValued=0;
}

void attributesInfo::setNumAttributes(int num)
{
	int i;

	numAttributes=num;
	numAttributesMC=num-1;
	typeOfAttributes=new int[num];
	valuesOfNominalAttributes=new JVector<JString *>[num];

	minDomain=new float[num-1];
	maxDomain=new float[num-1];
	sizeDomain=new float[num-1];
	sizeDomain2=new float[num-1];

	averageAttribute=new float[num-1];
	countValues=new int[num-1];	
	mostFrequentValueAttribute = new int[num-1];
	countValuesAttribute = new int *[num-1];

	nominalAttributes = new int[num-1];
	realAttributes = new int[num-1];
	numNominal = numReal = 0;

	for(i=0;i<num;i++) {
		typeOfAttributes[i]=-1;
	}
}


void attributesInfo::setTypeOfAttribute(int attribute,int type)
{
#ifdef DEBUG
	if(attribute<0 || attribute>=numAttributes ) {
		fprintf(stderr,"Incorrect values at attributesInfo::setTypeOfAttribute %d\n",attribute);
		exit(1);
	}

	if(typeOfAttributes[attribute]!=-1) {
		fprintf(stderr,"Already defined type for attribute %d\n",attribute);
		exit(1);
	}
#endif

	if(attribute<numAttributesMC) {
		if(type==REAL) {
			thereAreRealValued=1;
			realAttributes[numReal++] = attribute;
		}
		if(type==NOMINAL) {
			thereAreNominal=1;
			nominalAttributes[numNominal++] = attribute;
		}
	}
	typeOfAttributes[attribute]=type;

}


void attributesInfo::insertNominalValue(int attribute,JString *value)
{
#ifdef DEBUG
	if(attribute<0 || attribute>=numAttributes) {
		fprintf(stderr,"Incorrect value at attributesInfo::insertNominalValue %d\n",attribute);
		exit(1);
	}

	if(typeOfAttributes[attribute]!=NOMINAL) {
		fprintf(stderr,"Attribute %d is not nominal at attributesInfo::insertNominalValue %d\n",attribute);
		exit(1);
	}
#endif

	valuesOfNominalAttributes[attribute].addElement(value);
}

JString *attributesInfo::getNominalValue(int attribute,int value)
{
#ifdef DEBUG
	if(attribute<0 || attribute>=numAttributes) {
		fprintf(stderr,"Incorrect attr at attributesInfo::getNominalValue %d\n",attribute);
		exit(1);
	}
	if(typeOfAttributes[attribute]!=NOMINAL) {
		fprintf(stderr,"Attribute %d is not nominal at attributesInfo::insertNominalValue %d\n",attribute);
		exit(1);
	}
	if(value<0 || value>=valuesOfNominalAttributes[attribute].size()) {
		fprintf(stderr,"Incorrect value at attributesInfo::getNominalValue %d\n",value);
		exit(1);
	}
#endif

	return valuesOfNominalAttributes[attribute].elementAt(value);
}


int attributesInfo::getNumValuesAttribute(int attribute)
{
#ifdef DEBUG
	if(attribute<0 || attribute>=numAttributes) {
		fprintf(stderr,"Incorrect value at attributesInfo::getNumValuesAttribute %d\n",attribute);
		exit(1);
	}

	if(typeOfAttributes[attribute]!=NOMINAL) {
		fprintf(stderr,"Attribute %d is not nominal at attributesInfo::getNumValuesAttribute\n",attribute);
		exit(1);
	}
#endif

	return valuesOfNominalAttributes[attribute].size();
}

void attributesInfo::insertInstance(instance *ins)
{
    int i, j;

	//First sample
	if(!numExamples) {
		int numClasses=getNumValuesAttribute(numAttributes-1);
		classOfInstances=new int[numClasses];
		for(i=0;i<numClasses;i++) classOfInstances[i]=0;

		for(i=0;i<numAttributes-1;i++) {
			if(typeOfAttributes[i]==NOMINAL) {
				int num = getNumValuesAttribute(i);
				countValuesAttribute[i] = new int[num];	
				for(j=0; j<num;j++) countValuesAttribute[i][j]=0;
			} else {
				averageAttribute[i] = 0;
				countValues[i] = 0;
			}
		}
	}

	int instanceClass=ins->getClass();
	classOfInstances[instanceClass]++;

	if(ins->hasMissingValues()) {
		for(i=0;i<numAttributes-1;i++) {
			if(!ins->isMissing(i)) {
				if(typeOfAttributes[i]==NOMINAL) {
					countValuesAttribute[i][ins->valueOfAttribute(i)]++;
				} else {
					float value=ins->realValueOfAttribute(i);
					averageAttribute[i]+=value;
					countValues[i]++;
					if(!numExamples) {
						minDomain[i] = maxDomain[i] = value;
					} else {
						if(value<minDomain[i]) 
							minDomain[i]=value;
						if(value>maxDomain[i]) 
							maxDomain[i]=value;
					}
					sizeDomain[i]=maxDomain[i]-minDomain[i];
					sizeDomain2[i]=sizeDomain[i]/2;
				}
			}
		}
	} else {
		for(i=0;i<numAttributes-1;i++) {
			if(typeOfAttributes[i]==NOMINAL) {
				countValuesAttribute[i][ins->valueOfAttribute(i)]++;
			} else {
				float value=ins->realValueOfAttribute(i);
				averageAttribute[i]+=value;
				countValues[i]++;
				if(!numExamples) {
					minDomain[i] = maxDomain[i] = value;
				} else {
					if(value<minDomain[i]) 
						minDomain[i]=value;
					if(value>maxDomain[i]) 
						maxDomain[i]=value;
				}
				sizeDomain[i]=maxDomain[i]-minDomain[i];
				sizeDomain2[i]=sizeDomain[i]/2;
			}
		}
	}

	numExamples++;
}

void attributesInfo::calculateAverages() 
{
    int i,j;

	int numClasses=getNumValuesAttribute(numAttributes-1);

	mostFrequentClass=leastFrequentClass=0;
	int numMostFrequent=classOfInstances[0];
	int numLeastFrequent=classOfInstances[0];
	for(i=1;i<numClasses;i++) {
		if(classOfInstances[i]>numMostFrequent) {
			mostFrequentClass=i;
			numMostFrequent=classOfInstances[i];
		}
		if(classOfInstances[i]<numLeastFrequent) {
			leastFrequentClass=i;
			numLeastFrequent=classOfInstances[i];
		}
	}
	mb.printf("Least frequent class is %d\n",leastFrequentClass);
	mb.printf("Most frequent class is %d\n",mostFrequentClass);

	for(i=0;i<numAttributes-1;i++) {
		if(typeOfAttributes[i]==NOMINAL) {
			int maxGlob=countValuesAttribute[i][0];
			int bestValGlob=0;
			for(j=1;j<getNumValuesAttribute(i);j++) {
				if(countValuesAttribute[i][j]>maxGlob) {
					maxGlob=countValuesAttribute[i][j];
					bestValGlob=j;
				}
			}
			mostFrequentValueAttribute[i]=bestValGlob;
			//printf("Att %d %d\n",i,bestValGlob);
		} else {
			if(countValues[i]>0) {
				averageAttribute[i]/=(float)countValues[i];
			}
			//printf("Att %d %f\n",i,averageAttribute[i]);
		}
	}

	/*if(!thereAreRealValued) {
		cm.removeParameter(KR_ADI);
		cm.removeParameter(KR_HYPERRECT);
		cm.removeParameter(KR_INSTANCE_SET);
	}*/
}

float attributesInfo::getAverageOfAttribute(int attribute)
{
	return averageAttribute[attribute];
}

int attributesInfo::getMostFrequentValueOfAttribute(int attribute)
{
	return mostFrequentValueAttribute[attribute];
}

void attributesInfo::setBounds(float *min,float *max)
{
	int i;

	for(i=0;i<numAttributes-1;i++) {
		minDomain[i]=min[i];
		maxDomain[i]=max[i];
		sizeDomain[i]=max[i]-min[i];
		sizeDomain2[i]=sizeDomain[i]/2;
	}
}

int attributesInfo::valueOfNominalAttribute(int attribute,char *def) {
        int i;
        int value=-1;
        int values;
        JString tmp(def);

        if(attribute==numAttributes) values=getNumClasses();
        else values=getNumValuesAttribute(attribute);

        for(i=0;i<values && value==-1;i++) {
                if(getNominalValue(attribute,i)->equals(tmp)) value=i;
        }
	return value;
}

