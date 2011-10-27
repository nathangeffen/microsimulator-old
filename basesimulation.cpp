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

  // Normalize state parameter to the correct time period
  for (StateMap::iterator itState = states_.begin();
       itState != states_.end();
       ++itState) {
    itState->second->normalizeParameters(timePeriod_);
  }

  // Initialize individuals
  for (int i=0; i<nIndividuals_; ++i) {
    Individual individual;

    for (StateMap::iterator it = states_.begin(); it != states_.end(); ++it) {
      individual.setStateValue(it->first, it->second->initialize());
    }

    individuals_.push_back(individual);
  }
};

void BaseSimulation::simulate() {
  for (int i=0; i<nIterations_; ++i) {
    for (auto stateName : stateOrder_) {
      for (auto& individual : individuals_) {
        if (passesFilters(*individual.getStateValues(),
              *states_[stateName]->getFilterFunctions())) {
          double stateValue = individual.getStateValue(stateName);
          stateValue = states_[stateName]->transition(stateValue, states_,
              individuals_, individual);
          individual.setStateValue(stateName, stateValue);
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
    string stateName = get<1>(descriptor);
    vector<On> filters = get<3>(descriptor);

    IndividualVector filteredIndividuals(individuals_.size());
    auto itend = remove_copy_if(individuals_.begin(), individuals_.end(),
                   filteredIndividuals.begin(),
                   [&states_, &stateName, filters](Individual& i){
      return !passesFilters(*i.getStateValues(), filters);
    });
    filteredIndividuals.erase(itend, filteredIndividuals.end());

    o.description = outputDescription;
    o.value = get<2>(descriptor)(stateName, filteredIndividuals);
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


void BaseSimulation::addState(string name, State* const state,
          bool markForDeletion)
{
  states_[name] = state;
  stateOrder_.push_back(name);
  if (markForDeletion) {
    statesToDelete_.push_back(name);
  }
}

void BaseSimulation::addAnalysisDescriptor(string analysisName, string stateName,
    AnalysisFunction analysisFunction,  vector<On> filters)
{
  analysisDescriptors_.push_back(AnalysisDescriptor(analysisName,
      stateName, analysisFunction, filters));
}

BaseSimulation::~BaseSimulation()
{
  for (vector<string>::iterator it=statesToDelete_.begin();
       it!=statesToDelete_.end();
       ++it) {
    delete states_[*it];
  }
}
