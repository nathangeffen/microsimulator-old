/*
 * BaseSimulation.h
 *
 *  Created on: 12 Oct 2011
 *      Author: Nathan Geffen
 */

#ifndef MICROSIMULATOR_H_
#define MICROSIMULATOR_H_

#include <vector>
#include <map>
#include <string>
#include <tuple>

#include "simutils.h"
#include "individual.h"
#include "state.h"
#include "state_parameter.h"

namespace microsimulator {

const ParameterMap emptyParameterMap;
const StateMap emptyStateMap;
const FilterFunctionList emptyFilterFunctionVector;

class BaseSimulation
{
public:
  BaseSimulation (int nIndividuals=defaultNumberIndividuals,
              int nIterations=defaultNumberIterations,
              double timePeriod=YEAR,
              const StateMap& states=emptyStateMap) :
    nIndividuals_(nIndividuals),
    nIterations_(nIterations),
    timePeriod_(timePeriod),
    states_(states)   {};
  ~BaseSimulation();

  virtual void prepare();
  void simulate();
  AnalysisOutput analyze();

  // Getters and setters
  int getIndividuals() const;
  int getIterations() const;
  double getTimePeriod() const;
  ParameterMap getParameters() const;
  double getParameterValue(string name) const;
  double getParameterNormalizedValue(string name) const;
  double getParameterTimePeriod(string name) const;

  void setIndividuals(int nIndividuals_);
  void setIterations(int nIterations_);
  void setTimePeriod(double timePeriod_);
  void addState(string name, State* const state, bool markForDeletion=false);
  void addAnalysisDescriptor(string AnalysisName, string stateName,
      AnalysisFunction function,
      FilterFunctionList filters=emptyFilterFunctionVector);

protected:
  int nIndividuals_;
  int nIterations_;
  double timePeriod_;
  IndividualVector individuals_;
  StateMap states_;
  vector<string> stateOrder_;
  vector<string> statesToDelete_;
  AnalysisDescriptorList analysisDescriptors_;
};

}

#endif /* SIMULATION_H_ */
