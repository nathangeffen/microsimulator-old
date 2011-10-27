/*
 * state_parameter.h
 *
 *  Created on: 13 Oct 2011
 *      Author: nathan
 */

#ifndef STATE_PARAMETER_H_
#define STATE_PARAMETER_H_

#include "simutils.h"

namespace microsimulator {

class StateParameter {
public:
  StateParameter() : value_(0.0), timePeriod_(YEAR), normalizedValue_(0.0) {};
  StateParameter(double value,
                 double timePeriod,
                 NormalizeFunction normalizeFunction=
                     normalize_compounded_proportion) :
      value_(value),
      timePeriod_(timePeriod),
      normalizedValue_(value),
      normalizeFunction_(normalizeFunction)
      {};

  double getNormalizedValue() const;
  double getTimePeriod() const;
  double getValue() const;
  void setNormalizedValue(double normalizedValue);
  void setTimePeriod(double time_period);
  void setValue(double value);
  void setNormalizeFunction(NormalizeFunction normalizeFunction);
  double computeNormalizedValue(double toTimePeriod);
  double operator()() const {return getNormalizedValue(); };

protected:
  double value_;
  double timePeriod_;
  double normalizedValue_;
  double (*normalizeFunction_) (double, double, double);
};

}

#endif /* STATE_PARAMETER_H_ */
