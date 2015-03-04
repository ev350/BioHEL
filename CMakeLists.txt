cmake_minimum_required(VERSION 2.8.4)
project(BioHEL)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -I. -Wno-deprecated-register -Wno-logical-op-parentheses -Wno-parentheses")

set(SOURCE_FILES
random.cpp
classifier_gabil.cpp
classifierFitness.cpp
instanceSet.cpp
lex.yy.cpp
instance.cpp
timeManagement.cpp
JString.cpp
populationWrapper.cpp
timersManagement.cpp
ga.cpp
factory.cpp
attributesInfo.cpp
timerHierar.cpp
timerMDL.cpp
timerSymbolicKR.cpp
timerRealKR.cpp
agentPerformance.cpp
utils.cpp
timerGlobals.cpp
timerMutation.cpp
timerEvolutionStats.cpp
timerCrossover.cpp
classifier_hyperrect.cpp
mtwist.cpp
agentPerformanceTraining.cpp
classifier_hyperrect_list.cpp
windowingILAS.cpp
windowingGWS.cpp
classifier_hyperrect_list_real.cpp
classifier_hyperrect_list_plus.cpp
classifier_hyperrect_list_less.cpp
classifier_hyperrect_list_ratio.cpp
main.cpp
random.h
classifier.h
classifier_gabil.h
classifier_aggregated.h
classifierFitness.h
instanceSet.h
instance.h
timeManagement.h
JString.h
populationWrapper.h
timersManagement.h
ga.h
factory.h
attributesInfo.h
greater_than_test.h
timerHierar.h
timerMDL.h
timerSymbolicKR.h
timerRealKR.h
agentPerformance.h
utils.h
timerGlobals.h
timerMutation.h
timerEvolutionStats.h
timerCrossover.h
classifier_hyperrect.h
mtwist.h
agentPerformanceTraining.h
classifier_hyperrect_list.h
windowing.h
windowingILAS.h
windowingGWS.h
classifier_hyperrect_list_real.h
classifier_hyperrect_list_plus.h
classifier_hyperrect_list_less.h
classifier_hyperrect_list_ratio.h
)

add_executable(biohel ${SOURCE_FILES})

add_custom_target(
    flexything
    COMMAND flex -i -o lex.yy.cpp lex_conf.l
)

add_dependencies(biohel flexything)


