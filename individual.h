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
  void initializeStateValue(double value);
  void setStateValue(int stateIndex, double value);
  double getStateValue(int stateIndex) const;
  StateValueVector* getStateValues() {return &stateValues_;};

private:
  StateValueVector stateValues_;
};

}

#endif /* INDIVIDUAL_H_ */
