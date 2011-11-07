/*
 * individual.cpp
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#include "individual.h"

using namespace microsimulator;

void Individual::initializeStateValue(double value)
{
  stateValues_.push_back(value);
}

void Individual::setStateValue(int stateIndex, double value)
{
  stateValues_[stateIndex] = value;
}

double Individual::getStateValue(int stateIndex) const
{
  return stateValues_[stateIndex];
}
