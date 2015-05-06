cmake_minimum_required(VERSION 2.8.4)
project(BioHEL)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -I. -Wno-deprecated-register -Wno-logical-op-parentheses -Wno-parentheses")

set(SOURCE_FILES
agentPerformance.cpp
agentPerformance.h
agentPerformanceTraining.cpp
agentPerformanceTraining.h
attributesInfo.cpp
attributesInfo.h
bounds_test.h
classifier.h
classifier_aggregated.h
classifier_gabil.cpp
classifier_gabil.h
classifier_hyper_list.cpp
classifier_hyper_list.h
classifier_hyperrect.cpp
classifier_hyperrect.h
classifier_hyperrect_list.cpp
classifier_hyperrect_list.h
configCodes.h
testFactory.h
classifierFitness.cpp
classifierFitness.h
factory.cpp
factory.h
ga.cpp
ga.h
greater_than_test.h
instance.cpp
instance.h
instanceSet.cpp
instanceSet.h
JString.cpp
JString.h
JVector.h
less_than_test.h
lex.yy.cpp
lex_conf.h
lex_conf.l
main.cpp
mtwist.cpp
mtwist.h
populationWrapper.cpp
populationWrapper.h
random.cpp
random.h
ratio_test.h
test.h
timeManagement.cpp
timeManagement.h
timerCrossover.cpp
timerCrossover.h
timerEvolutionStats.cpp
timerEvolutionStats.h
timerGlobals.cpp
timerGlobals.h
timerHierar.cpp
timerHierar.h
timerMDL.cpp
timerMDL.h
timerMutation.cpp
timerMutation.h
timerRealKR.cpp
timerRealKR.h
timersManagement.cpp
timersManagement.h
timerSymbolicKR.cpp
timerSymbolicKR.h
utils.cpp
utils.h
windowing.h
windowingGWS.cpp
windowingGWS.h
windowingILAS.cpp
windowingILAS.h
)

add_executable(biohel ${SOURCE_FILES})

add_custom_target(
    flexything
    COMMAND flex -i -o lex.yy.cpp lex_conf.l
)

add_dependencies(biohel flexything)


