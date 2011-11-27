/*
 * tb_states.cpp
 *
 *  Created on: 17 Oct 2011
 *      Author: nathan
 */

#include <iostream>

#include "simutils.h"
#include "individual.h"
#include "tb_states.h"

using namespace microsimulator;

StateActiveTb::StateActiveTb(
      StateParameter active_tb_risk,
      StateParameter active_tb_cure) :
          active_tb_risk_(active_tb_risk),
          active_tb_cure_(active_tb_cure)
{
  stateParameters_.insert(pair<int, StateParameter*>(
      Parameter(ACTIVE_TB_RISK),
      &active_tb_risk_));
  stateParameters_.insert(pair<int, StateParameter*>(
      Parameter(ACTIVE_TB_CURE),
      &active_tb_cure_));
  aliveStateIndex_ = -1;
  numberActiveTbIndex_ = -1;
}


void StateActiveTb::registerRequiredState(string stateName,
                                                  int stateIndex)
{
  if ( stateName == "alive" ) {
    aliveStateIndex_ = stateIndex;
    filterFunctions_.push_back(On(aliveStateIndex_));
  } else
  if ( stateName == "number active tb" ) {
    numberActiveTbIndex_ = stateIndex;
  }
}

void StateActiveTb::prepare(double timePeriod) throw(SimulationException)
{
  if ( numberActiveTbIndex_ == -1 ) {
    throw SimulationException("Please register 'number active tb' state with 'active tb' state.");
  }
  if ( aliveStateIndex_ == -1 ) {
    throw SimulationException("Please register 'alive' state with 'active tb' state.");
  }

  filterFunctions_.push_back(On(aliveStateIndex_));
  State::prepare(timePeriod);
}


double StateActiveTb::transition(double value, StateVector& states,
    IndividualVector& individuals, Individual& individual)
{
  if (!value) {
    if (frand() < PARM(ACTIVE_TB_RISK)) {
      double noTimesInfected = individual.getStateValue(numberActiveTbIndex_);
      individual.setStateValue(numberActiveTbIndex_, noTimesInfected + 1.0);
      return 1;
    } else {
      return 0;
    }
  } else {
    if (frand() < PARM(ACTIVE_TB_CURE)) {
      return 0;
    } else {
      return 1;
    }
  }
}

/////////////////


StateAlive::StateAlive(
    StateParameter age_0_60,
    StateParameter age_61,
    StateParameter tb_risk_age_0_60,
    StateParameter tb_risk_age_61) :
    age_0_60_(age_0_60),
    age_61_(age_61),
    tb_risk_age_0_60_(tb_risk_age_0_60),
    tb_risk_age_61_(tb_risk_age_61)
{
  tbStateIndex_ = ageStateIndex_ = aliveStateIndex_ = -1;
  stateParameters_.insert(pair<int, StateParameter*>(
      Parameter(AGE_0_60), &age_0_60_));

  stateParameters_.insert(pair<int, StateParameter*>(
      Parameter(AGE_61), &age_61_));

  stateParameters_.insert(pair<int, StateParameter*>(
      Parameter(TB_RISK_AGE_0_60), &tb_risk_age_0_60_));

  stateParameters_.insert(pair<int, StateParameter*>(
      Parameter(TB_RISK_AGE_61), &tb_risk_age_61_));
  initializeFunction_ = always_true;
}

void StateAlive::registerRequiredState(string stateName, int stateIndex)
{
  if ( stateName == "active tb" ) {
    tbStateIndex_ = stateIndex;
  } else
  if ( stateName == "age"  ) {
    ageStateIndex_ = stateIndex;
  } else
  if ( stateName == "alive" ) {
    aliveStateIndex_ = stateIndex;
  }
}

void StateAlive::prepare(double timePeriod) throw(SimulationException)
{
  if ( aliveStateIndex_ == -1 )
    aliveStateIndex_ = id_;
  if ( ageStateIndex_ == -1 ) {
    throw SimulationException("Please register 'age' state with 'alive' state.");
  }
  if ( ageStateIndex_ == -1 ) {
    throw SimulationException("Please register 'age' state with 'alive' state.");
  }
  if ( tbStateIndex_ == -1 ) {
    throw SimulationException("Please register 'active tb' state with 'alive' state.");
  }

  filterFunctions_.push_back(On(aliveStateIndex_));
  State::prepare(timePeriod);
}

double StateAlive::transition(double value, StateVector& states,
    IndividualVector& individuals, Individual& individual)
{
  if (!value) {// Once you're dead, you're dead
    return 0.0;
  }

  // 0-60 and no TB
  if (individual.getStateValue(tbStateIndex_) == 0) {
    if (individual.getStateValue(ageStateIndex_) <= 60) {
      if(frand() < PARM(AGE_0_60)) {
        return 0.0;
      } else {
        return 1.0;
      }
    }
  }

  // No TB and age >60
  if (individual.getStateValue(tbStateIndex_) == 0) {
    if (individual.getStateValue(ageStateIndex_) > 60) {
      if(frand() < PARM(AGE_61)) {
        return 0.0;
      } else {
        return 1.0;
      }
    }
  }

  // TB and age <= 60
  if (individual.getStateValue(ageStateIndex_) <= 60) {
    if(frand() < PARM(TB_RISK_AGE_0_60)) {
        return 0.0;
    } else {
        return 1.0;
    }
  }

  // TB and age > 60
  if (individual.getStateValue(ageStateIndex_) > 60) {
    if(frand() < PARM(TB_RISK_AGE_61)) {
        return 0.0;
    } else {
        return 1.0;
    }
  }


  // This shouldn't happen - keep alive.
  return 1.0;
}
