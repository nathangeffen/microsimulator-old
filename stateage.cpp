/*
 * stateage.cpp
 *
 *  Created on: 16 Nov 2011
 *      Author: nathan
 */

#include "simutils.h"
#include "stateage.h"

using namespace microsimulator;

StateAge::StateAge(StateParameter ageIncrement) :
      ageIncrement_(ageIncrement)
{
  MATCH(AGE_INCREMENT, ageIncrement_);
  aliveStateIndex_ = -1;
}

double StateAge::transition(double value, StateVector& states,
    IndividualVector& individuals, Individual& individual)
{
  if (transitionFunction_) {
    return transitionFunction_(value, states, individuals, individual);
  }

  return value + PARM(AGE_INCREMENT);
}

void StateAge::registerRequiredState(string stateName, int stateIndex)
{
  if ( stateName == "alive" ) {
    if (aliveStateIndex_ > -1) {
      // Put code here to delete the alive state filter
    }
    aliveStateIndex_ = stateIndex;
    filterFunctions_.push_back(On(aliveStateIndex_));
  }
}


