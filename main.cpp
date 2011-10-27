/*
 * main.cpp
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#include <iostream>
#include <cstdlib>

#include "basesimulation.h"
#include "state.h"
#include "tb_states.h"

using namespace std;

using namespace microsimulator;


int main(int argc, char *argv[]) {

////  if (argc < 3) {
////    cout << argv[0] << "<number of individuals> <number of iterations> "
////         << "<iteration period in days>" << endl;
////    return 1;
////  }
////
////  int nIndividuals = atoi(argv[1]);
////  int nIterations = atoi(argv[2]);
////  double timePeriod = atoi(argv[3]);
////
////  Simulation simulation(nIndividuals, nIterations, timePeriod);
////  simulation.initialize();
////  simulation.simulate();
////  cout << "Simulation run" << endl;
//
////  double normalize_compounded_proportion(double proportion,
////                                         double fromTimePeriod,
////                                         double toTimePeriod);
//
//
//  // State Parameter tests
//
//  StateParameter defaultTransition(0.009, YEAR);
//  StateParameter defaultTransitionBack(0.7, YEAR, normalize_linear_proportion);
//
//  cout << "defaultTransition value: " << defaultTransition.getValue() << endl;
//  cout << "defaultTransition normalized value: "
//       << defaultTransition.getNormalizedValue() << endl;
//
//  cout << "defaultTransition normalized value from overloaded function: "
//       << defaultTransition() << endl;
//
//
//  cout << "defaultTransition normalized to a month: "
//       << defaultTransition.computeNormalizedValue(MONTH) << endl;
//
//  cout << "defaultTransition getNormalizedValue: "
//       << defaultTransition.getNormalizedValue() << endl;
//
//  cout << "defaultTransition getNormalizedValue from overloaded function: "
//       << defaultTransition() << endl;
//
//  cout << "defaultTransition value: " << defaultTransition.getValue() << endl;
//
//  cout << "defaultTransitionBack normalized to 2 years: "
//       << defaultTransitionBack.computeNormalizedValue(YEAR * 2.0) << endl;
//
//  cout << "defaultTransitionBack getNormalizedValue: "
//       << defaultTransitionBack.getNormalizedValue() << endl;
//
//  cout << "defaultTransitionBack value: " << defaultTransitionBack.getValue()
//       << endl;
//
//  ParameterMap parameters;
//  parameters.insert(pair<string, StateParameter>("transition",
//      defaultTransition));
//  parameters.insert(pair<string, StateParameter>("transition_back",
//      defaultTransitionBack));
//
//  // State tests
//
//  StateMap sm;
//  IndividualVector iv;
//  Individual it;
//
//  StateAge stateAge;
//
//  cout << "State Age " << stateAge.transition(10.0, sm, iv, it) << endl;
//
//  State *pStateAge = &stateAge;
//
//  pStateAge->normalizeParameters(MONTH);
//  cout << "State Age pointer: " << pStateAge->transition(10.0, sm, iv, it) << endl;
//
////  for (int i=0; i<1000; i++) {
////    individual.setStateValue("default", defaultState.transition(
////           individual.getStateValue("default"),
////           states,
////           parameters));
////    cout << "Default state value: "
////         << individual.getStateValue("default")
////         << endl;
////  }

//  cout << "Setting up simulation" << endl;
//  AnalysisDescriptor descriptor;
//  AnalysisOutput output;
//  BaseSimulation s;
//  s.setTimePeriod(MONTH);
//  s.setIndividuals(10);
//  s.setIterations(5);
//  cout << "Preparing simulation" << endl;
//  s.prepare();
//  cout << "Simulating" << endl;
//  s.simulate();
//  cout << "Simulation finished" << endl;
//
//  descriptor.push_back(pair<string, AnalysisFunction>("age", count));
//  output = s.analyze(descriptor);
//
//  cout << "DESCRIPTIONS" << endl;
//  for (AnalysisOutput::iterator it=output.begin(); it!=output.end(); ++it) {
//    cout << it->first << " " << it->second << endl;
//  }
//

  StateActiveTb activeTb;
  StateAlive alive;
  StateAge age;
  State numberActiveTb;

  BaseSimulation s1;

  s1.addState("alive", &alive);
  s1.addState("active tb", &activeTb);
  s1.addState("number active tb", &numberActiveTb);
  s1.addState("age", &age);

  s1.setTimePeriod(MONTH);
  s1.setIndividuals(10000);
  s1.setIterations(75*12);

  cout << "Preparing tb simulation" << endl;
  s1.prepare();
  cout << "Simulating tb" << endl;
  s1.simulate();
  cout << "Simulation tb finished" << endl;

  AnalysisOutput s1Output;

  s1.addAnalysisDescriptor("Number of people alive with tb",
      "active tb", count, {On("alive"), On("active tb")});
  s1.addAnalysisDescriptor("Number of people who died with tb",
      "active tb", count, {Off("alive")});
  s1.addAnalysisDescriptor("Mean age of the living",
      "age", mean, {On("alive")});
  s1.addAnalysisDescriptor("Mean age of the dead",
      "age", mean, {Off("alive")});
  s1.addAnalysisDescriptor("Median age of the living",
      "age", median, {On("alive")});
  s1.addAnalysisDescriptor("Median age of the dead",
      "age", median, {Off("alive")});
  s1.addAnalysisDescriptor("Number of people alive", "alive", count);
  s1.addAnalysisDescriptor("Number of living people who've had active TB",
      "number active tb", count, {On("alive")});
  s1.addAnalysisDescriptor("Number of people who've had active TB",
      "number active tb", count);
  s1.addAnalysisDescriptor("Mean number of times people with active TB infected",
      "number active tb", mean, {On("number active tb")});
  s1Output = s1.analyze();

  cout << "DESCRIPTIONS" << endl;
  for (auto it : s1Output) {
    cout << it.description << " " << it.value<< endl;
  }

  cout << "END" << endl;

  return 0;
}
