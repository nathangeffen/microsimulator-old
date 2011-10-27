/*
 * state.cpp
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#include <string>
#include <iostream>

#include "individual.h"
#include "state_parameter.h"
#include "state.h"

using namespace std;
using namespace microsimulator;

void State::setInitializeFunction(const InitializeFunction f)
{
  this->initializeFunction_ = f;
}

double State::initialize() {
  if (initializeFunction_) {
    return initializeFunction_();
  }
  return 0.0;
}

void State::normalizeParameters(double timePeriod)
{
  map <int, StateParameter*>::iterator it;
  for (it=stateParameters_.begin(); it!= stateParameters_.end(); ++it){
    it->second->computeNormalizedValue(timePeriod);
  }
}


double State::transition(double value, StateMap& states,
    IndividualVector& individuals, Individual& individual)
{
  if (transitionFunction_) {
    return transitionFunction_(value, states, individuals, individual);
  }
  return value;
}

void State::setTransitionFunction(const TransitionFunction f)
{
  this->transitionFunction_ = f;
}

void State::setParameterValue(Parameter parameter, double value)
{
  stateParameters_[parameter]->setValue(value);
}

void State::addFilterFunction(On o)
{
  filterFunctions_.push_back(o);
}

double State::getParameterValue(Parameter parameter) const
{
  map <int, StateParameter*>::const_iterator it;
  it = stateParameters_.find(parameter);
  if (it != stateParameters_.end())
  {
    return it->second->getValue();
  }
  return 0.0;
}

double State::getParameterNormalizedValue(Parameter parameter) const
{
  map <int, StateParameter*>::const_iterator it;
  it = stateParameters_.find(parameter);
  if (it != stateParameters_.end())
  {
    return it->second->getNormalizedValue();
  }
  return 0.0;
}


FilterFunctionList* State::getFilterFunctions()
{
  return &filterFunctions_;
}

/////////////////////

StateAge::StateAge(StateParameter ageIncrement) :
      ageIncrement_(ageIncrement)
{
  MATCH(AGE_INCREMENT, ageIncrement_);
  filterFunctions_.push_back(On("alive"));
}


double StateAge::transition(double value, StateMap& states,
    IndividualVector& individuals, Individual& individual)
{
  if (transitionFunction_) {
    return transitionFunction_(value, states, individuals, individual);
  }

  return value + PARM(AGE_INCREMENT);
}
