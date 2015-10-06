/*
 * ITrickle.h
 *
 *  Created on: Oct 5, 2015
 *      Author: carlo
 */

#ifndef TRICKLEBASE_H_
#define TRICKLEBASE_H_

#include <omnetpp.h>

class RplEngine;

class ITrickle {


protected:

    RplEngine* rpl;

public:
    ITrickle();
    virtual ~ITrickle();

    // Called when a reset is needed
    virtual void reset() = 0;

    // Called when a consistant packet is received
    virtual void dioReceived() = 0;

    // Function to set pointer to RPL instance for callback
    void setRplPtr(RplEngine* r);

};

#endif /* TRICKLEBASE_H_ */
