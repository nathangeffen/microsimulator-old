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
const StateVector emptyStateVector;
const FilterFunctionList emptyFilterFunctionVector;

class BaseSimulation
{
public:
  BaseSimulation (int nIndividuals=defaultNumberIndividuals,
              int nIterations=defaultNumberIterations,
              double timePeriod=YEAR,
              const StateVector& states=emptyStateVector) :
    nIndividuals_(nIndividuals),
    nIterations_(nIterations),
    timePeriod_(timePeriod),
    states_(states),
    stateCounter_(0) {};
  virtual ~BaseSimulation();

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
  int addState(string name, State* const state, bool markForDeletion=false);
  void addAnalysisDescriptor(string AnalysisName,
                             int stateIndex,
                             AnalysisFunction function,
                             FilterFunctionList filters=
                                                     emptyFilterFunctionVector);

protected:
  int nIndividuals_;
  int nIterations_;
  double timePeriod_;
  IndividualVector individuals_;
  StateVector states_;
  vector<int> statesToDelete_;
  AnalysisDescriptorVector analysisDescriptors_;
private:
  int stateCounter_;
};

}

#endif /* SIMULATION_H_ */
