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
#include <set>

#include "simutils.h"
#include "state_parameter.h"

using namespace std;

namespace microsimulator {

class State
{
public:
  enum Parameter {
    DEFAULT_PARAMETER=0
  };
  State() : initializeFunction_(0), transitionFunction_(0) {};
  virtual double initialize();
  void setInitializeFunction(const InitializeFunction f);
  virtual double transition(double value, StateMap& states,
      IndividualVector& individuals, Individual& individual);
  void normalizeParameters(double timePeriod);
  void setTransitionFunction(const TransitionFunction f);
  void setParameterValue(Parameter parameter, double value);
  void addFilterFunction(On o);
  double getParameterValue(Parameter parameter) const;
  double getParameterNormalizedValue(Parameter parameter) const;
  FilterFunctionList* getFilterFunctions();

protected:
  InitializeFunction initializeFunction_;
  TransitionFunction transitionFunction_;
  FilterFunctionList filterFunctions_;
  map <int, StateParameter*> stateParameters_;
};


class StateAge : public State
{
public:
  enum Parameter {
    AGE_INCREMENT=1
  };
  StateAge(StateParameter ageIncrement=
      StateParameter(1, YEAR, normalize_linear_proportion));
  virtual double transition(double value, StateMap& states,
      IndividualVector& individuals, Individual& individual);

protected:
  StateParameter ageIncrement_;
};

}

#endif /* STATE_H_ */
