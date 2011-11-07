/*
 * tb_states.h
 *
 *  Created on: 17 Oct 2011
 *      Author: nathan
 */

#ifndef TB_STATES_H_
#define TB_STATES_H_

#include "simutils.h"
#include "state.h"

namespace microsimulator {

class StateActiveTb : public State
{
public:
  enum Parameter {
    ACTIVE_TB_RISK,
    ACTIVE_TB_CURE
  };
  StateActiveTb(
      StateParameter active_tb_risk=
          StateParameter(0.01, YEAR),
      StateParameter active_tb_cure=
          StateParameter(0.7, YEAR));
  virtual void registerRequiredState(string stateName, int stateIndex);
  virtual void prepare(double timePeriod) throw(UnregisteredState);
  virtual double transition(double value, StateVector& states,
      IndividualVector& individuals, Individual& individual);

protected:
  StateParameter active_tb_risk_;
  StateParameter active_tb_cure_;
  int aliveStateIndex_;
  int numberActiveTbIndex_;
};


class StateAlive : public State
{
public:
  enum Parameter {
    AGE_0_60,
    AGE_61,
    TB_RISK_AGE_0_60,
    TB_RISK_AGE_61
  };
  StateAlive(
      StateParameter age_0_60=
          StateParameter(0.001, YEAR),
      StateParameter age_61=
          StateParameter(0.2, YEAR),
      StateParameter tb_risk_age_0_60=
          StateParameter(0.7, YEAR),
      StateParameter tb_risk_age_61=
          StateParameter(0.8, YEAR)
  );
  virtual void registerRequiredState(string stateName, int stateIndex);
  virtual void prepare(double timePeriod) throw(UnregisteredState);
  double transition(double value, StateVector& states,
      IndividualVector& individuals, Individual& individual);

protected:
  StateParameter age_0_60_;
  StateParameter age_61_;
  StateParameter tb_risk_age_0_60_;
  StateParameter tb_risk_age_61_;
  int ageStateIndex_;
  int aliveStateIndex_;
  int tbStateIndex_;
};

}

#endif /* TB_STATES_H_ */
