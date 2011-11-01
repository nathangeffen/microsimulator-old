/*
 * simutils.h
 *
 *  Created on: 16 Oct 2011
 *      Author: nathan
 */

#ifndef SIMUTILS_H_
#define SIMUTILS_H_

#include <string>
#include <vector>
#include <map>
#include <random>
#include <iostream>

#define PARM(X) (*stateParameters_[Parameter(X)])()
#define MATCH(X, Y)   stateParameters_.insert(pair<int, StateParameter*>( \
                        Parameter(X), \
                        &Y))

using namespace std;


namespace microsimulator {

struct OutputDescriptor {
  string description;
  double value;
};

class StateParameter;
class State;
class Individual;
class On;



typedef map<string, StateParameter> ParameterMap;
typedef map<string, State*> StateMap;
typedef map<string, double> StateValueMap;
//typedef bool (*FilterFunction)(const StateValueMap&);
typedef vector <On> FilterFunctionList;
typedef double (*InitializeFunction)();
typedef double (*NormalizeFunction)(double, double, double);
typedef vector<Individual> IndividualVector;
typedef double (*AnalysisFunction) (string&, IndividualVector&);
typedef tuple <string, string, AnalysisFunction, FilterFunctionList>
              AnalysisDescriptor;
typedef vector < AnalysisDescriptor > AnalysisDescriptorList;
typedef vector<OutputDescriptor> AnalysisOutput;
typedef double (*TransitionFunction) (double, StateMap&, IndividualVector&,
    Individual&);

const double SECOND = 1.0 / (24.0 * 60.0 * 60.0);
const double MINUTE = 1.0 / (24.0 * 60.0);
const double HOUR = 1.0 / 24.0;
const double DAY = 1.0;
const double WEEK = 7.0;
const double MONTH = 365.0 / 12.0;
const double YEAR = 365.0;
const int defaultNumberIndividuals = 10000;
const int defaultNumberIterations = 70;
static mt19937 mersenne_twister;


// Utility functions
double always_true();
double frand(uniform_real_distribution<> dist=uniform_real_distribution<>(0,1));


template <class FilterContainer>
bool passesFilters(const StateValueMap& stateValueMap,
                   const FilterContainer& filters)
{
  for (auto f : filters) {
    if (not f(stateValueMap)) {
      return false;
    }
  }
  return true;
}

// Common filters

class On
{
public:
  On(const string& stateName, bool on=true) : stateName_(stateName), on_(on) {};
  bool operator()(const StateValueMap& stateValues) const;
protected:
  string stateName_;
  bool on_;
};

#define Off(x) On(x, false)

double normalize_linear_proportion(double proportion,
                                   double fromTimePeriod,
                                   double toTimePeriod);
double normalize_compounded_proportion(double proportion,
                                       double fromTimePeriod,
                                       double toTimePeriod);

double count(string& stateName, IndividualVector& individuals);
double mean(string& stateName, IndividualVector& individuals);
double median(string& stateName, IndividualVector& individuals);

double defaultTransitionFunction(double value,
                                 StateMap& states,
                                 ParameterMap& parameters);
double aliveStateTransition(double value,
                                 StateMap& states,
                                 ParameterMap& parameters);
double ageStateTransition(double value,
                          StateMap& states,
                          ParameterMap& parameters);
}

#endif /* SIMUTILS_H_ */
