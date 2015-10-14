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

// Basic class for NODE event collector

class INET_API EventCollector : public cSimpleModule  {

private:

    //////////// Utility data       /////////////

    std::string directory; // Output path
    bool start; // True if trace is active
    bool timeline; // True if stat timeline is active
    bool periodic; // True if stat periodic collection is active
    double period; // Period for periodic stat collection

    //////////// Utility functions /////////////

    // Trace value over time
    void traceValue(std::string metric, int id, double value, int id2);

    // Trace value periodically over time
    void tracePeriodicValue(std::string metric, int id, double value, int id2);

    // Store final value
    void finalValue(std::string metric, int id, double value, int id2 = -1);

public:
    EventCollector();
    virtual ~EventCollector();

    void initialize(int stage);

};

#endif /* EVENTCOLLECTIONS_H_ */
