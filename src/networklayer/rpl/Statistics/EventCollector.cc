/*
 * EventCollections.cc
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#include <EventCollector.h>

#include <sstream>
#include <string>
#include <fstream>


Register_Abstract_Class(EventCollector);

EventCollector::EventCollector() {
    id = 0;
}

EventCollector::~EventCollector() {
    // TODO Auto-generated destructor stub
}

void EventCollector::initialize(int stage){
    if(stage==0){
        // Local variable initialization
        start = false;

        // Get params
        timeline = par("timeline").boolValue();
        periodic = par("periodic").boolValue();
        period = par("period").doubleValue();
        directory = par("directory").stringValue();

    }
}

void EventCollector::setID(int i){
    id = i;
}


void EventCollector::traceValue(std::string metric, double value, int id2){
    if(!timeline)
        return;

    // Build filename
    int run =simulation.getActiveEnvir()->getConfigEx()->getActiveRunNumber();
    std::ostringstream name;
    if( id2 == -1 )
        name << directory << "trace_" << metric << "_" << id << "_" << run << ".log";
    else
        name << directory << "trace_" << metric << "_" << id << "_" << id2 << "_" << run << ".log";

    // Build output
    std::ostringstream print;

    print << simTime().dbl() << " " << value << endl;

    std::ofstream logfile(name.str().c_str(), std::ios::out | std::ios::app);
    if (logfile.is_open()){

        logfile<< print.str().c_str();
        logfile.close();
    }
}


void EventCollector::tracePeriodicValue(std::string metric, double value, int id2){
    if(!periodic)
        return;

    // Build filename
    int run =simulation.getActiveEnvir()->getConfigEx()->getActiveRunNumber();
    std::ostringstream name;
    if( id2 == -1 )
        name << directory << "periodic_" << metric << "_" << id << "_" << run << ".log";
    else
        name << directory << "periodic_" << metric << "_" << id << "_" << id2 << "_" << run << ".log";

    // Build output
    std::ostringstream print;

    print << simTime().dbl() << " " << value << endl;

    std::ofstream logfile(name.str().c_str(), std::ios::out | std::ios::app);
    if (logfile.is_open()){

        logfile<< print.str().c_str();
        logfile.close();
    }
}


void EventCollector::finalValue(std::string metric, double value, int id2){
    // Build filename
    int run =simulation.getActiveEnvir()->getConfigEx()->getActiveRunNumber();
    std::ostringstream name;
    if( id2 == -1 )
        name << directory << metric << "_" << id << ".log";
    else
        name << directory << metric << "_" << id << "_" << id2 << ".log";

    // Build output
    std::ostringstream print;

    print << run << " " << value << endl;

    std::ofstream logfile(name.str().c_str(), std::ios::out | std::ios::app);
    if (logfile.is_open()){

        logfile<< print.str().c_str();
        logfile.close();
    }
}


