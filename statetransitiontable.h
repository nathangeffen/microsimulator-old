/*
 * statetransitiontable.h
 *
 *  Created on: 16 Nov 2011
 *      Author: Nathan Geffen
 *
 *
 */

#ifndef STATETRANSITIONTABLE_H_
#define STATETRANSITIONTABLE_H_

#include "simutils.h"
#include "state.h"
#include "state_parameter.h"

using namespace std;

namespace microsimulator {

enum MatchFunction {
  EQ,
  GTE_LTE,
  GTE_LT,
  GT_LTE,
  GT_LT
};

enum AssignFunction {
  ASSIGN,
  INCREMENT
};

struct TransitionEntry
{
  int stateIndex;
  MatchFunction matchFunction;
  double lower;
  double upper;
};

struct TransitionRecord
{
  vector < TransitionEntry > entries;
  AssignFunction assignFunction;

  double probability;
  double probabilityTimePeriod;
  double normalizedProbability;
  NormalizeFunction probabilityNormalizeFunction;

  double newValue;
  double valueTimePeriod;
  double normalizedValue;
  NormalizeFunction valueNormalizeFunction;
};

class StateTransitionTable : public State
{
public:
  void loadStateTransitionTable(string inputTable,
                                SimulationFormat tableFormat);
  double transitionUsingTransitionTable(double value,
                                        StateVector& states,
                                        IndividualVector& individuals,
                                        Individual& individual);
  virtual void prepare(double timePeriod);

protected:
  vector < TransitionRecord > transitionTable_;

private:
  void loadJsonTransitionTable(string inputTable);
};

}

#endif /* STATETRANSITIONTABLE_H_ */
