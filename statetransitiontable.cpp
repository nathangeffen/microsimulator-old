/*
 * statetransitiontable.cpp
 *
 *  Created on: 16 Nov 2011
 *      Author: nathan
 */

#include "simutils.h"
#include "statetransitiontable.h"
#include "simplejson/JSON.h"

using namespace microsimulator;

void StateTransitionTable::loadStateTransitionTable(
                                                 string inputTable,
                                                 SimulationFormat tableFormat)
{
  switch( tableFormat ) {
    case JSON:
      loadJsonTransitionTable(inputTable);
      break;
    case SQLITE:
      throw SimulationException("SQLITE state transition table"
                                "not implemented yet", __LINE__, __FILE__);
      break;
    default:
      throw SimulationException("Unknown state transition table format",
                                __LINE__, __FILE__);
      break;
  }
}

void StateTransitionTable::loadJsonTransitionTable(string inputTable)
{
  // Parse example data
  JSONValue *value = JSON::Parse(inputTable.c_str());

  // Did it go wrong?
  if (value == NULL)
  {
    throw SimulationException("JSON transition table did not parse",
                              __LINE__, __FILE__);
  }
  else
  {
    // Retrieve the main object
    JSONObject root;
  }
}



double StateTransitionTable::transitionUsingTransitionTable(
                                      double value,
                                      StateVector& states,
                                      IndividualVector& individuals,
                                      Individual& individual)
{
  for ( auto record : transitionTable_ ) {
    bool match = false;
    for ( auto entry : record.entries ) {
      switch ( entry.matchFunction ) {
        case ( EQ ):
          if ( value == entry.lower )
            match = true;
          break;
        case ( GTE_LTE ):
          if ( value >= entry.lower && value <= entry.upper )
            match = true;
        case ( GTE_LT ):
          if ( value >= entry.lower && value < entry.upper )
            match = true;
          break;
        case ( GT_LTE ):
          if ( value > entry.lower && value <= entry.upper )
            match = true;
          break;
        case ( GT_LT ):
          if ( value > entry.lower && value < entry.upper )
            match = true;
          break;
      }
      if ( !match ) break;
    }
    if ( match ) {
      if ( frand() < record.normalizedProbability ) {
        if ( record.assignFunction == ASSIGN ) {
          return record.normalizedValue;
        } else {
          return value + record.normalizedValue;
        }
      } else {
        // Event did not occur, then just return the state value unchanged
        return value;
      }
    }
  }
  // No match, then just return the state value unchanged.
  return value;
}

void StateTransitionTable::prepare(double toTimePeriod)
{
  for ( auto record : transitionTable_ ) {

    if ( record.valueTimePeriod != toTimePeriod ) {
      record.normalizedValue = record.valueNormalizeFunction(
                                                         record.newValue,
                                                         record.valueTimePeriod,
                                                         toTimePeriod );
    } else {
      record.normalizedValue = record.newValue;
    }

    if ( record.probabilityTimePeriod != toTimePeriod ) {
      record.normalizedProbability = record.probabilityNormalizeFunction(
                                                   record.probability,
                                                   record.probabilityTimePeriod,
                                                   toTimePeriod );
    } else {
      record.normalizedProbability = record.probability;
    }

  }
}
