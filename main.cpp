#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

#include "configManagement.h"
#include "populationWrapper.h"
#include "classifierFitness.h"
#include "random.h"
#include "instanceSet.h"
#include "timersManagement.h"
#include "attributesInfo.h"
#include "lex_conf.h"
#include "timeManagement.h"
#include "messageBuffer.h"
#include "classifier_aggregated.h"

int stop = 0;
messageBuffer mb;
attributesInfo ai;
configManagement cm;
instanceSet *is;
timeManagement timeM;
Random rnd;
double percentageOfLearning = 0;
int lastIteration = 0;

void handler(int sig)
{
	stop = 1;
}

classifier *runGA(timersManagement & timers)
{
	stop = 0;
	lastIteration = 0;
	percentageOfLearning = 0;

	populationWrapper pw((int) cm.getParameter(POP_SIZE));

	timers.setPW(&pw);
	signal(SIGINT, handler);

	double ave1, ave2;
	pw.getAverageAccuracies(ave1, ave2);
	mb.printf("Initial population average accuracy1:%f accuracy2:%f\n",
		  ave1, ave2);
	int countIt = 0, numIterations = (int) cm.getParameter(ITERATIONS);
	if (numIterations == 0)
		stop = 1;

	for (; !stop;) {
		timers.incIteration(lastIteration);

		if (is->newIteration(lastIteration)) {
			pw.activateModifiedFlag();
		}

		pw.gaIteration();

		//classifier *best = pw.getBestPopulation();
		//char pheno[10000];
		//best->dumpPhenotype(pheno);
		//mb.printf("Best\n%s",pheno);
		//classifierFitness2(*best);	

		timers.dumpStats();
		countIt++;

		if (countIt == numIterations)
			stop = 1;
		else if (countIt == numIterations - 1)
			lastIteration = 1;
		percentageOfLearning = (double) countIt
		    / (double) (numIterations);
	}

	if (stop && countIt < numIterations)
		return NULL;

	classifier *ind = pw.getBestOverall();
	mb.printf("Best acc %f %f\n", ind->getAccuracy(),
		  ind->getAccuracy2());
	classifier *clone = pw.cloneClassifier(ind, 0);
	//clone->postprocess();
	//clone->fitnessComputation();
	clone->adjustFitness();
	//printf("Postprocessed acc %f %f\n",clone->getAccuracy(),clone->getAccuracy2());
	return clone;
}

int main(int argc, char *argv[])
{
	if (argc < 4) {
		fprintf(stderr, "Incorrect parameters\n"
			"%s: <Config file> <Train file> <Test file>\n",
			argv[0]);
		exit(1);
	}

	if (argc == 5)
		mb.setFile(argv[4]);

	parseConfig(argv[1]);
	rnd.dumpSeed();
	is = new instanceSet(argv[2], TRAIN);
	timersManagement timers;

	int optimizationMethod = (int) cm.getParameter(MAX_MIN);
	classifierFactory cf;
	int countRepeat = 0;

	int maxRules = -1;
	if (cm.thereIsParameter(MAX_RULES))
		maxRules = (int) cm.getParameter(MAX_RULES);

	classifier_aggregated ruleSet;

	do {
		int cancelled = 0;
		classifier *best = NULL;
		for (int i = 0; i < tGlobals->numRepetitionsLearning; i++) {
			classifier *bestIt = runGA(timers);
			if (!bestIt) {
				cancelled = 1;
				break;
			}

			if (best == NULL
			    || bestIt->compareToIndividual(best,
							   optimizationMethod)
			    > 0) {
				if (best)
					cf.deleteClassifier(best);
				best = bestIt;
			}

			if (i < tGlobals->numRepetitionsLearning - 1) {
				is->restart();
				timers.reinit();
			}

		}
		if (cancelled)
			break;

		if (isMajority(*best)) {
			char phenotype[2000000];
			best->dumpPhenotype(phenotype);
			mb.printf("Rule:%s", phenotype);
			is->removeInstancesAndRestart(best);
			ruleSet.addClassifier(best);
			classifierBriefTest(ruleSet, is);
			countRepeat = 0;
			timers.reinit();
			if (is->getNumInstances() == 0)
				break;

			if (maxRules != -1
			    && ruleSet.getNumClassifiers() == maxRules) {
				ruleSet.setDefaultRule(is);
				break;
			}

		} else {
			countRepeat++;
			cf.deleteClassifier(best);
			if (countRepeat == 3) {
				ruleSet.setDefaultRule(is);
				break;
			} else {
				is->restart();
				timers.reinit();
			}
		}
	} while (1);

	delete is;

	char phenotype[2000000];
	ruleSet.dumpPhenotype(phenotype);
	mb.printf("Phenotype: \n%s\n", phenotype);

	classifierStats(ruleSet, argv[2], "Train");
	classifierStats(ruleSet, argv[3], "Test");

	return 0;
}
