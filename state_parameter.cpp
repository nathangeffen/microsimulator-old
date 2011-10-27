/*
 * state_parameter.cpp
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#include <cmath>
#include "state_parameter.h"

using namespace microsimulator;

double StateParameter::computeNormalizedValue(double toTimePeriod) {
  if (timePeriod_ != toTimePeriod) {
    normalizedValue_ = normalizeFunction_( value_,
                                           timePeriod_,
                                           toTimePeriod);
  } else {
    normalizedValue_ = value_;
  }
  return normalizedValue_;
}

double StateParameter::getNormalizedValue() const
{
  return normalizedValue_;
}

double StateParameter::getTimePeriod() const
{
    return timePeriod_;
}

double StateParameter::getValue() const
{
    return value_;
}

void StateParameter::setNormalizedValue(double normalizedValue)
{
    this->normalizedValue_ = normalizedValue;
}

void StateParameter::setTimePeriod(double timePeriod)
{
    this->timePeriod_ = timePeriod;
}

void StateParameter::setValue(double value)
{
    this->value_ = value;
}

void StateParameter::setNormalizeFunction(NormalizeFunction normalizeFunction)
{
  this->normalizeFunction_ = normalizeFunction;
}
