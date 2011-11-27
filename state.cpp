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

State::State(string name, int id) :
    initializeFunction_(0),
    transitionFunction_(0),
    name_(name),
    id_(id)
{};

double State::initialize() {
  if (initializeFunction_) {
    return initializeFunction_();
  }
  return 0.0;
}

double State::transition(double value, StateVector& states,
    IndividualVector& individuals, Individual& individual)
{
  if (transitionFunction_) {
    return transitionFunction_(value, states, individuals, individual);
  }
  return value;
}

void State::prepare(double timePeriod)
{
  this->normalizeParameters(timePeriod);
}

void State::normalizeParameters(double timePeriod)
{
  map <int, StateParameter*>::iterator it;
  for (it=stateParameters_.begin(); it!= stateParameters_.end(); ++it){
    it->second->computeNormalizedValue(timePeriod);
  }
}

void State::addFilterFunction(On o)
{
  filterFunctions_.push_back(o);
}

void State::setInitializeFunction(const InitializeFunction f)
{
  this->initializeFunction_ = f;
}

void State::setTransitionFunction(const TransitionFunction f)
{
  this->transitionFunction_ = f;
}

void State::setParameterValue(Parameter parameter, double value)
{
  stateParameters_[parameter]->setValue(value);
}

void State::setName(const string& name)
{
  name_ = name;
}

void State::setId(int id)
{
  id_ = id;
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

string State::getName() const
{
  return name_;
}

int State::getId() const
{
  return id_;
}

FilterFunctionList* State::getFilterFunctions()
{
  return &filterFunctions_;
}
