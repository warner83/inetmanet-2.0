/*
 * ITrickle.h
 *
 *  Created on: Oct 5, 2015
 *      Author: carlo
 */

#ifndef ITRICKLE_H_
#define ITRICKLE_H_

// This class is the interface for Trickle implementations

#include <omnetpp.h>

#include "INETDefs.h"

#include "NodeEventCollector.h"

class INET_API ITrickle: public cSimpleModule {

protected:

    NodeEventCollector* ec;

    void signalEngine(int kind);

public:
    ITrickle();
    virtual ~ITrickle();

    void initialize(int stage);

    // Called when a reset is needed
    virtual void reset() = 0;

    // Called when a consistant packet is received
    virtual void dioReceived() = 0;

};

#endif /* ITRICKLE_H_ */
