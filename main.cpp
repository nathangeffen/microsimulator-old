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
#include "stateage.h"
#include "tb_states.h"

using namespace std;

using namespace microsimulator;


int main(int argc, char *argv[]) {

  StateActiveTb activeTb;
  StateAlive alive;
  StateAge age;
  State numberActiveTb;

  BaseSimulation s1;

  if (argc < 3) {
    cout << argv[0] << "<number of individuals> <number of iterations> "
         << "<iteration period in days>" << endl;
    return 1;
  }

  int nIndividuals = atoi(argv[1]);
  int nIterations = atoi(argv[2]);
  double timePeriod = atof(argv[3]);

  cout << "Individuals: " << nIndividuals << endl;
  cout << "Iterations: " << nIterations << endl;
  cout << "Time Period: " << timePeriod << endl;

  s1.addState("alive", &alive);
  s1.addState("active tb", &activeTb);
  s1.addState("number active tb", &numberActiveTb);
  s1.addState("age", &age);
  alive.registerRequiredState("active tb", activeTb.getId());
  alive.registerRequiredState("age", age.getId());
  alive.registerRequiredState("alive", alive.getId());
  activeTb.registerRequiredState("alive", alive.getId());
  activeTb.registerRequiredState("number active tb", numberActiveTb.getId());
  age.registerRequiredState("alive", alive.getId());



  s1.setTimePeriod(timePeriod);
  s1.setIndividuals(nIndividuals);
  s1.setIterations(nIterations);

  cout << "Preparing tb simulation" << endl;
  s1.prepare();
  cout << "Simulating tb" << endl;
  s1.simulate();
  cout << "Simulation tb finished" << endl;

  AnalysisOutput s1Output;

  s1.addAnalysisDescriptor("Number of people alive with tb",
      activeTb.getId(), count, {On(alive.getId()), On(activeTb.getId())});
  s1.addAnalysisDescriptor("Number of people who died with tb",
      activeTb.getId(), count, {Off(alive.getId())});
  s1.addAnalysisDescriptor("Mean age of the living",
      age.getId(), mean, {On(alive.getId())});
  s1.addAnalysisDescriptor("Mean age of the dead",
      age.getId(), mean, {Off(alive.getId())});
  s1.addAnalysisDescriptor("Median age of the living",
      age.getId(), median, {On(alive.getId())});
  s1.addAnalysisDescriptor("Median age of the dead",
      age.getId(), median, {Off(alive.getId())});
  s1.addAnalysisDescriptor("Number of people alive", alive.getId(), count);
  s1.addAnalysisDescriptor("Number of living people who've had active TB",
      numberActiveTb.getId(), count, {On(alive.getId())});
  s1.addAnalysisDescriptor("Number of people who've had active TB",
      numberActiveTb.getId(), count);
  s1.addAnalysisDescriptor("Mean number of times people with active TB infected",
      numberActiveTb.getId(), mean, {On(numberActiveTb.getId())});
  s1Output = s1.analyze();

  cout << "DESCRIPTIONS" << endl;
  for (auto it : s1Output) {
    cout << it.description << " " << it.value << endl;
  }

  cout << "END" << endl;

  return 0;
}
