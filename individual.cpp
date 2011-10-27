/*
 * individual.cpp
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#include "individual.h"

using namespace microsimulator;

void Individual::initializeStateValue(string stateName, double value)
{
  stateValues_.insert(pair<string, double>(stateName, value));
}

void Individual::setStateValue(string stateName, double value)
{
  stateValues_[stateName] = value;
}

double Individual::getStateValue(string stateName) const
{

  auto it = stateValues_.find(stateName);
  if (it != stateValues_.end())
  {
    return it->second;
  } else {
    return 0.0;
  }
}
