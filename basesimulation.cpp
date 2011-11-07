//============================================================================
// Name        : microsimulator.cpp
// Author      : Nathan Geffen
// Version     :
// Copyright   : Copyright Nathan Geffen, 2011
// Description : Microsimulator of epidemics
//============================================================================

#include <vector>
#include <iostream>
#include <algorithm>

#include "simutils.h"
#include "basesimulation.h"

using namespace microsimulator;

const double DEFAULT_TRANSITION_PROBABILITY = 0.009;
const double DEFAULT_TRANSITION_BACK_PROBABILITY = 0.7;

void BaseSimulation::prepare()
{

  // If there are no states, insert a default StateAge

  if (states_.size() == 0) {
    addState("age", new StateAge(), true);
  }

  // Prepare states
  for ( auto itState : states_ ) {
    itState->prepare(timePeriod_);
  }


  // Initialize individuals
  for ( int i=0; i<nIndividuals_; ++i ) {
    Individual individual;

    for ( auto state : states_ ) {

      individual.initializeStateValue(state->initialize());
    }

    individuals_.push_back(individual);
  }

};

void BaseSimulation::simulate() {
  for (int i=0; i<nIterations_; ++i) {
    for (auto& individual : individuals_) {
      for (int j=0; j < stateCounter_; ++j) {
        if (passesFilters(*individual.getStateValues(),
                          *states_[j]->getFilterFunctions())) {
          double stateValue = individual.getStateValue(j);
          stateValue = states_[j]->transition(stateValue,
                                              states_,
                                              individuals_,
                                              individual);
          individual.setStateValue(j, stateValue);
        }
      }
    }
  }
};

AnalysisOutput BaseSimulation::analyze()
{
  AnalysisOutput output;
  OutputDescriptor o;

  for (auto descriptor : analysisDescriptors_) {

    // Get state name and filter out unneeded individuals
    string outputDescription = get<0>(descriptor);
    int stateIndex = get<1>(descriptor);
    vector<On> filters = get<3>(descriptor);

    IndividualVector filteredIndividuals(individuals_.size());
    auto itend = remove_copy_if(
                   individuals_.begin(), individuals_.end(),
                   filteredIndividuals.begin(),
                   [filters](Individual& i){
      return !passesFilters(*i.getStateValues(), filters);
    });
    filteredIndividuals.erase(itend, filteredIndividuals.end());

    o.description = outputDescription;
    o.value = get<2>(descriptor)(stateIndex, filteredIndividuals);
    output.push_back(o);
  }

  o.description = "population";
  o.value = (double) individuals_.size();
  output.push_back(o);

  return output;
};


// Getters and setters

int BaseSimulation::getIndividuals() const
{
  return nIndividuals_;
}

int BaseSimulation::getIterations() const
{
  return nIterations_;
}

double BaseSimulation::getTimePeriod() const
{
  return timePeriod_;
}

void BaseSimulation::setIndividuals(int nIndividuals_)
{
  this->nIndividuals_ = nIndividuals_;
}

void BaseSimulation::setIterations(int nIterations_)
{
  this->nIterations_ = nIterations_;
}

void BaseSimulation::setTimePeriod(double timePeriod_)
{
  this->timePeriod_ = timePeriod_;
}


int BaseSimulation::addState(string name,
                             State* state,
                             bool markForDeletion)
{
  int stateCounter = stateCounter_;
  state->setName(name);
  state->setId(stateCounter);
  states_.push_back(state);
  if (markForDeletion) {
    statesToDelete_.push_back(stateCounter);
  }
  ++stateCounter_;
  return stateCounter;
}

void BaseSimulation::addAnalysisDescriptor(
            string analysisName,
            int stateIndex,
            AnalysisFunction analysisFunction,
            vector<On> filters)
{
  analysisDescriptors_.push_back(AnalysisDescriptor(analysisName,
      stateIndex, analysisFunction, filters));
}

BaseSimulation::~BaseSimulation()
{
  for (auto index : statesToDelete_) {
    delete states_[index];
  }
}
