/*
 * LinkEstimatorBase.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef LINKESTIMATORBASE_H_
#define LINKESTIMATORBASE_H_

#include <cownedobject.h>

#include <omnetpp.h>

#include "IPv6Datagram.h"

// This class defines the interface for the link estimator modules

class LinkEstimatorBase : public cOwnedObject {
public:
    LinkEstimatorBase();
    virtual ~LinkEstimatorBase();

    // Return the link cost
    virtual double getLinkCost(IPv6Address node) = 0;

    // Signal packet loss ratio
    virtual double setPacketLoss(IPv6Address node, double packetLoss) = 0;
};

#endif /* LINKESTIMATORBASE_H_ */
