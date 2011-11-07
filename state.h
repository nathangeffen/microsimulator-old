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
  State(string name="", int id=0) :
      initializeFunction_(0),
      transitionFunction_(0),
      name_(name),
      id_(id)
  {};
  virtual double initialize();
  void setInitializeFunction(const InitializeFunction f);
  virtual double transition(double value, StateVector& states,
      IndividualVector& individuals, Individual& individual);
  //virtual StateDescriptorVector listRequiredStates();
  virtual void registerRequiredState(string stateName, int indexTo) {};
  virtual void prepare(double timePeriod);
  void normalizeParameters(double timePeriod);
  void setTransitionFunction(const TransitionFunction f);
  void setParameterValue(Parameter parameter, double value);
  void setName(const string& name);
  void setId(int id);
  void addFilterFunction(On o);
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


class StateAge : public State
{
public:
  enum Parameter {
    AGE_INCREMENT=1
  };
  StateAge(StateParameter ageIncrement=
      StateParameter(1, YEAR, normalize_linear_proportion));
  virtual double transition(double value, StateVector& states,
      IndividualVector& individuals, Individual& individual);
  virtual void registerRequiredState(string stateName, int stateIndex);
protected:
  StateParameter ageIncrement_;
private:
  int aliveStateIndex_;
};

}

#endif /* STATE_H_ */
