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

class INET_API ITrickle: public cSimpleModule {

protected:
    void signalEngine(int kind);

public:
    ITrickle();
    virtual ~ITrickle();

    // Called when a reset is needed
    virtual void reset() = 0;

    // Called when a consistant packet is received
    virtual void dioReceived() = 0;

};

#endif /* ITRICKLE_H_ */
