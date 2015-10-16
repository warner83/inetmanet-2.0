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

// Basic class for NODE event collector

class INET_API EventCollector : public cSimpleModule  {

protected:

    //////////// Utility data       /////////////

    std::string directory; // Output path
    bool start; // True if trace is active
    bool timeline; // True if stat timeline is active
    bool periodic; // True if stat periodic collection is active
    double period; // Period for periodic stat collection
    int id; // Module ID

    //////////// Utility functions /////////////

    // Trace value over time
    void traceValue(std::string metric, double value, int id2 = -1);

    // Trace value periodically over time
    void tracePeriodicValue(std::string metric, double value, int id2 = -1);

    // Store final value
    void finalValue(std::string metric, double value, int id2 = -1);

public:
    EventCollector();
    virtual ~EventCollector();

    void initialize(int stage);

    // Set ID for stats storage
    void setID(int i);

};

#endif /* EVENTCOLLECTIONS_H_ */
