/*
 * state.h
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#ifndef STATE_H_
#define STATE_H_

#include <map>
#include <vector>

#include "simutils.h"
#include "state_parameter.h"

using namespace std;

namespace microsimulator {

// This class encapsulates a simulation state. A Simulation has multiple states.
// A simulation also consists of multiple individuals.
// Each individual in the simulation has a value for each state.

class State
{
public:

  // Parameters used by the State are stored in the Parameter enumeration
  enum Parameter {
    DEFAULT_PARAMETER=0
  };
  State( string name="", int id=0 );
  virtual ~State() {};

  // Determine the initial value for a state for a particular individual.
  virtual double initialize();

  // Given a value for an individual's state, this method calculates what the
  // new value should be.
  virtual double transition(double value,
                            StateVector& states,
                            IndividualVector& individuals,
                            Individual& individual);

  virtual void registerRequiredState(string stateName, int indexTo) {};
  virtual void prepare(double timePeriod);
  void normalizeParameters(double timePeriod);

  void addFilterFunction(On o);

  void setInitializeFunction(const InitializeFunction f);
  void setTransitionFunction(const TransitionFunction f);
  void setParameterValue(Parameter parameter, double value);
  void setName(const string& name);
  void setId(int id);

  double getParameterValue(Parameter parameter) const;
  double getParameterNormalizedValue(Parameter parameter) const;
  string getName() const;
  int getId() const;
  FilterFunctionList* getFilterFunctions();

protected:
  InitializeFunction initializeFunction_;
  TransitionFunction transitionFunction_;
  FilterFunctionList filterFunctions_;
  map <int, StateParameter*> stateParameters_;
  string name_;
  int id_;
};

}

#endif /* STATE_H_ */
