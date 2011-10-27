/*
 * individual.h
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <string>
#include <map>

#include "simutils.h"
#include "state.h"

namespace microsimulator {

class Individual
{
public:
  Individual() {};
  void initializeStateValue(string stateName, double value);
  void setStateValue(string stateName, double value);
  double getStateValue(string StateName) const;
  StateValueMap* getStateValues() {return &stateValues_;};

private:
  StateValueMap stateValues_;
};

}

#endif /* INDIVIDUAL_H_ */
