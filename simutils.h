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
#include <exception>

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

struct StateDescriptor {
  string stateName;
  string description;
};

class StateParameter;
class State;
class Individual;
class On;



typedef map<string, StateParameter> ParameterMap;
typedef int StateIndex; // Not yet in use
typedef vector<State*> StateVector;
typedef map<string, double> StateValueMap;
//typedef bool (*FilterFunction)(const StateValueMap&);
typedef vector <On> FilterFunctionList;
typedef double (*InitializeFunction)();
typedef double (*NormalizeFunction)(double, double, double);
typedef vector<Individual> IndividualVector;
typedef double (*AnalysisFunction) (int, IndividualVector&);
typedef tuple <string, int, AnalysisFunction, FilterFunctionList>
              AnalysisDescriptor;
typedef vector < AnalysisDescriptor > AnalysisDescriptorVector;
typedef vector<OutputDescriptor> AnalysisOutput;
typedef double (*TransitionFunction) (double, StateVector&, IndividualVector&,
    Individual&);
typedef vector <double> StateValueVector;
typedef vector <StateDescriptor> StateDescriptorVector;

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
bool passesFilters(const StateValueVector& stateValueVector,
                   const FilterContainer& filters)
{
  for (auto f : filters) {
    if (not f(stateValueVector)) {
      return false;
    }
  }
  return true;
}

// Common filters

class On
{
public:
  On(int stateIndex, bool on=true) : stateIndex_(stateIndex), on_(on) {};
  bool operator()(const StateValueVector& stateValues);
protected:
  int stateIndex_;
  bool on_;
};

#define Off(x) On(x, false)

double normalize_linear_proportion(double proportion,
                                   double fromTimePeriod,
                                   double toTimePeriod);
double normalize_compounded_proportion(double proportion,
                                       double fromTimePeriod,
                                       double toTimePeriod);

double count(int stateIndex, IndividualVector& individuals);
double mean(int stateIndex, IndividualVector& individuals);
double median(int stateIndex, IndividualVector& individuals);

double defaultTransitionFunction(double value,
                                 StateVector& states,
                                 ParameterMap& parameters);
double aliveStateTransition(double value,
                                 StateVector& states,
                                 ParameterMap& parameters);
double ageStateTransition(double value,
                          StateVector& states,
                          ParameterMap& parameters);
}

// Exceptions

class UnregisteredState: public exception
{
  virtual const char* what() const throw()
  {
    return "A state has not been registered";
  }
};

#endif /* SIMUTILS_H_ */
