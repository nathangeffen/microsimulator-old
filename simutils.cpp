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

double microsimulator::frand(uniform_real_distribution<> dist)
{
  return dist(mersenne_twister);
}

double microsimulator::always_true()
{
  return 1.0;
}


// Common filters

bool On::operator()(const StateValueMap& stateValues) const
{
  auto it = stateValues.find(stateName_);
  if ( it != stateValues.end() ) {
    if (it->second) {
      return (on_) ? true : false; // On returns true && Off returns false
    }
  }
  return (on_) ? false : true; // On returns false && Off returns true
}

double microsimulator::mean(string& stateName,
                                          IndividualVector& individuals)

{
  double output = 0.0;
  int count = 0;
  for (IndividualVector::iterator it = individuals.begin();
       it != individuals.end();
       ++it) {
    ++count;
    output += it->getStateValue(stateName);
  }
  output /= count;
  return output;
}

double microsimulator::median(string& stateName,
                                            IndividualVector& individuals)

{
  sort(individuals.begin(), individuals.end(),
    [&stateName](const Individual& a, const Individual& b) {
      return a.getStateValue(stateName) < b.getStateValue(stateName);
    });

  int size = individuals.size();
  double output = 0.0;
  if (!size)
    output = 0.0;
  else if (size == 1)
    output = individuals[0].getStateValue(stateName);
  else if (size % 2 == 0)
      output = (individuals[size / 2].getStateValue(stateName) +
        individuals[size / 2 + 1].getStateValue(stateName)) / 2.0;
  else
    output = individuals[size / 2 + 1].getStateValue(stateName);

  return output;
}


double microsimulator::count(string& stateName,
                                           IndividualVector& individuals)

{
  int output = 0;
  for (IndividualVector::iterator it = individuals.begin();
       it != individuals.end();
       ++it) {
    if (it->getStateValue(stateName)) {
      ++output;
    }
  }
  string description = "count:" + stateName;
  return  output;
}

double microsimulator::defaultTransitionFunction(double value,
                                 StateMap& states,
                                 ParameterMap& parameters)
{
  if (value) {
    if (parameters["transition_back"]() < frand()) {
      return 0.0;
    }
  }

  if (parameters["transition"]() < frand()) {
    return 1.0;
  } else {
    return 0.0;
  };
}

double microsimulator::aliveStateTransition(double value,
                                 StateMap& states,
                                 ParameterMap& parameters)
{
  if (parameters["probability_death"]() < frand() ) {
    return 0.0;
  } else {
    return value;
  }
}

double microsimulator::ageStateTransition(double value,
                                 StateMap& states,
                                 ParameterMap& parameters)
{
  return value + parameters["age_increment"]();
}
