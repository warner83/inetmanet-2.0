/*
 * EventCollections.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef EVENTCOLLECTIONS_H_
#define EVENTCOLLECTIONS_H_

#include "INETDefs.h"

#include <omnetpp.h>

#include <string.h>

#include "IPv6ControlInfo.h"

// Basic class for NODE event collector

class INET_API EventCollector : public cSimpleModule  {

    cMessage *periodic_collection;

protected:

    //////////// Utility data       /////////////

    std::string outDir; // Output path
    bool start; // True if trace is active
    bool timeline; // True if stat timeline is active
    bool periodic; // True if stat periodic collection is active
    double period; // Period for periodic stat collection
    bool onlyOmnetStats; // True if stat has to be saved only in omnetpp format

    int id; // Module ID

    // Omnetpp signals
    std::map<std::string, simsignal_t> statSignals;

    //////////// Utility functions /////////////

    // Trace value over time
    void traceValue(std::string metric, double value, int id2 = -1);

    // Trace value periodically over time
    void tracePeriodicValue(std::string metric, double value, int id2 = -1);

    // Store final value
    void finalValue(std::string metric, double value, int id2 = -1);

    // Register a new signal
    void registerRplStatSignal(std::string name);

    // Return true if a signal has been registered
    bool registeredRplStatSignal(std::string name);

    // Called periodically if periodic stat collection is enabled
    virtual void periodicStatCollection() {}

public:
    EventCollector();
    virtual ~EventCollector();

    void initialize(int stage);

    void handleMessage(cMessage *msg);

};

#endif /* EVENTCOLLECTIONS_H_ */
