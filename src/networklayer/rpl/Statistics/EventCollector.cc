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
    periodic_collection = NULL;
}

#define periodic_collection_kind_self_message 1

EventCollector::~EventCollector() {
    if( periodic_collection != NULL )
        cancelAndDelete(periodic_collection);
}

void EventCollector::initialize(int stage){

    // Initialize and set timer for periodic stat collection
    periodic_collection = new cMessage();
    periodic_collection->setKind(periodic_collection_kind_self_message);

    scheduleAt(simTime()+period, periodic_collection );
}

void EventCollector::setID(int i){
    id = i;
}

void EventCollector::registerRplStatSignal(std::string name){

    // Create a new signal

    simsignal_t signal = registerSignal(name.c_str());

    cProperty *statisticTemplate =
        getProperties()->get("statisticTemplate", "rplStats");
    ev.addResultRecorders(this, signal, name.c_str(), statisticTemplate);

    statSignals[name] = signal;
}

bool EventCollector::registeredRplStatSignal(std::string name){
    // Check if the signal has been registered already
    return statSignals.find(name) != statSignals.end();
}


void EventCollector::traceValue(std::string metric, double value, int id2){
    if(!timeline)
        return;

    // Omnet stat system

    std::string metricOmnet = metric;

    if( id2 != -1 ){
        // Append second ID if any
        std::ostringstream ss;
        ss << metric << "_" << id2;
        metricOmnet = ss.str();
    }

    if( !registeredRplStatSignal(metricOmnet) )
        registerRplStatSignal(metricOmnet);

    emit(statSignals[metricOmnet], value);

    if(onlyOmnetStats)
        return;

    // Build filename
    int run =simulation.getActiveEnvir()->getConfigEx()->getActiveRunNumber();
    std::ostringstream name;
    if( id2 == -1 )
        name << outDir << "trace_" << metric << "_" << id << "_" << run << ".log";
    else
        name << outDir << "trace_" << metric << "_" << id << "_" << id2 << "_" << run << ".log";

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

    // Omnet stat system

    std::string metricOmnet = metric;

    std::ostringstream ss;

    ss << "periodic_" << metric;

    if( id2 != -1 ){
        // Append second ID if any
        ss << "_" << id2;
    }

    metricOmnet = ss.str();

    if( !registeredRplStatSignal(metricOmnet) )
        registerRplStatSignal(metricOmnet);

    emit(statSignals[metricOmnet], value);

    if(onlyOmnetStats)
        return;

    // Build filename
    int run =simulation.getActiveEnvir()->getConfigEx()->getActiveRunNumber();
    std::ostringstream name;
    if( id2 == -1 )
        name << outDir << "periodic_" << metric << "_" << id << "_" << run << ".log";
    else
        name << outDir << "periodic_" << metric << "_" << id << "_" << id2 << "_" << run << ".log";

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

    // Omnet stat system

    if( id2 == -1 ){
        recordScalar(metric.c_str(), value);
    } else {
        std::ostringstream ss;
        ss << metric << "_" << id2;
        recordScalar(ss.str().c_str(), value);
    }

    if( onlyOmnetStats )
        return;

    // Build filename
    int run =simulation.getActiveEnvir()->getConfigEx()->getActiveRunNumber();
    std::ostringstream name;
    if( id2 == -1 )
        name << outDir << metric << "_" << id << ".log";
    else
        name << outDir << metric << "_" << id << "_" << id2 << ".log";

    // Build output
    std::ostringstream print;

    print << run << " " << value << endl;

    std::ofstream logfile(name.str().c_str(), std::ios::out | std::ios::app);
    if (logfile.is_open()){

        logfile<< print.str().c_str();
        logfile.close();
    }
}

void EventCollector::handleMessage(cMessage *msg){

    // Call the specific periodic collector
    periodicStatCollection();

    cancelEvent(periodic_collection);
    scheduleAt(simTime()+period, periodic_collection);
}


