#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#define TRAIN 1
#define TEST 2

class instance {
	int numAttributes;
	int traintest;
	char *missing;
	int id;
	int missingValues;

	int noRealAtt;

	void parseInstance(char *string);
	int extractNominalValue(char *instance,int attribute);
	int extractRealValue(float instance,int attribute);

public:
	//unsigned char *nominalValues;
	float *realValues;
	int instanceClass;
	instance(int id,char *string,int pTraintest);
	~instance();
	inline int valueOfAttribute(int attribute){return (unsigned char)realValues[attribute];}
	inline int getClass(){return instanceClass;}
	inline int hasMissingValues(){return missingValues;}
	void updateMissing();
	inline int isMissing(int atr){return missing[atr];}
	inline float realValueOfAttribute(int attribute){return realValues[attribute];}
	int getID(){return id;}
	void dumpInstance();
	void normalize();

	float distance(instance *other);
};

#endif
