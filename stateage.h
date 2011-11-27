/*
 * stateage.h
 *
 *  Created on: 16 Nov 2011
 *      Author: nathan
 */

#ifndef STATEAGE_H_
#define STATEAGE_H_

#include "simutils.h"
#include "state.h"
#include "state_parameter.h"

using namespace std;

namespace microsimulator {


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

#endif /* STATEAGE_H_ */
