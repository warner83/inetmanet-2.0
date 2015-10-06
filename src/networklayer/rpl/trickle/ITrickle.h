/*
 * ITrickle.h
 *
 *  Created on: Oct 5, 2015
 *      Author: carlo
 */

#ifndef ITRICKLE_H_
#define ITRICKLE_H_

#include <omnetpp.h>

#include "INETDefs.h"


class RplEngine;

class INET_API ITrickle {


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

#endif /* ITRICKLE_H_ */
