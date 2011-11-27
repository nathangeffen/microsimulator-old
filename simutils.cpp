/*
 * simutils.cpp
 *
 *  Created on: 16 Oct 2011
 *      Author: nathan
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "simutils.h"
#include "state_parameter.h"
#include "state.h"
#include "individual.h"

using namespace std;

using namespace microsimulator;

double microsimulator::normalize_linear_proportion(double proportion,
                                                   double fromTimePeriod,
                                                   double toTimePeriod)
{
  return proportion * (toTimePeriod / fromTimePeriod);
}



double microsimulator::normalize_compounded_proportion(double proportion,
                                                       double fromTimePeriod,
                                                       double toTimePeriod)
{
  double n = fromTimePeriod / toTimePeriod;
  double c = proportion + 1.0;
  // x^n = c ; we must solve for x
  // n*ln(x) = ln(c)
  // ln(x) = ln(c)/n
  // x = exp(ln(c)/n)
  // Finally subtract the 1 that was added in the previous statement
  return exp(log(c)/n) - 1;
}

double normalize_identity(double proportion,
                          double fromTimePeriod,
                          double toTimePeriod)
{
  return proportion;
}


double microsimulator::frand(uniform_real_distribution<> dist)
{
  return dist(mersenne_twister);
}

double microsimulator::always_true()
{
  return 1.0;
}


// Common filters

bool On::operator()(const StateValueVector& stateValues)
{
  if (stateValues[stateIndex_]) {
      return (on_) ? true : false; // On returns true && Off returns false
  }

  return (on_) ? false : true; // On returns false && Off returns true
}

double microsimulator::mean(int stateIndex,
                            IndividualVector& individuals)

{
  double output = 0.0;
  for (auto it : individuals) {
    output += it.getStateValue(stateIndex);
  }
  output /= individuals.size();
  return output;
}

double microsimulator::median(int stateIndex,
                              IndividualVector& individuals)

{
  sort(individuals.begin(), individuals.end(),
    [&stateIndex](const Individual& a, const Individual& b) {
      return a.getStateValue(stateIndex) < b.getStateValue(stateIndex);
    });

  int size = individuals.size();
  double output = 0.0;
  if (!size)
    output = 0.0;
  else if (size == 1)
    output = individuals[0].getStateValue(stateIndex);
  else if (size % 2 == 0)
      output = (individuals[size / 2].getStateValue(stateIndex) +
        individuals[size / 2 + 1].getStateValue(stateIndex)) / 2.0;
  else
    output = individuals[size / 2 + 1].getStateValue(stateIndex);

  return output;
}


double microsimulator::count(int stateIndex,
                             IndividualVector& individuals)

{
  int output = 0;
  for (auto it : individuals) {
    if (it.getStateValue(stateIndex)) {
      ++output;
    }
  }
  return  output;
}

double microsimulator::defaultTransitionFunction(double value,
                                 StateVector& states,
                                 ParameterMap& parameters)
{
  if (value) {
    if (parameters["transition_back"]() < frand()) {
      return 0.0;
    } else {
      return 1.0;
    }
  }

  if (parameters["transition"]() < frand()) {
    return 1.0;
  } else {
    return 0.0;
  };
}

double microsimulator::aliveStateTransition(double value,
                                 StateVector& states,
                                 ParameterMap& parameters)
{
  if (parameters["probability_death"]() < frand() ) {
    return 0.0;
  } else {
    return value;
  }
}

double microsimulator::ageStateTransition(double value,
                                 StateVector& states,
                                 ParameterMap& parameters)
{
  return value + parameters["age_increment"]();
}


SimulationException::SimulationException(string message,int line, string file)
{
    message_ = message;
    line_ = line;
    file_ = file;
    ::exception();
}

const char* SimulationException::what() const throw()
{
  stringstream message;

  message << message_ << " ";
  if ( line_ && file_ != "" )
    message << "Error at line " << line_ << " in " << file_;
  return message.str().c_str();
}

