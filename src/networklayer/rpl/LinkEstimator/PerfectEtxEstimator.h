/*
 * PerfectEtxEstimator.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef PERFECTETXESTIMATOR_H_
#define PERFECTETXESTIMATOR_H_

#include <LinkEstimatorBase.h>

#include "Coord.h"

class PerfectEtxEstimator: public LinkEstimatorBase {

    // Topology structure
    cTopology topo;

    // Retrieve node coordinates
    Coord getCoord(int index);

    // Function to convert addresses to index
    unsigned int addrToIndex(IPv6Address a);

public:
    PerfectEtxEstimator();
    virtual ~PerfectEtxEstimator();

    // The link cost is 1 for hop count
    virtual double getLinkCost(IPv6Address node);

    virtual double setPacketLoss(IPv6Address node, double packetLoss) {}  // Perfect link estimator does not use link information
};

#endif /* PERFECTETXESTIMATOR_H_ */
