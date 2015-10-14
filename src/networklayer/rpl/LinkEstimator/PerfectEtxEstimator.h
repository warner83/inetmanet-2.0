/*
 * PerfectEtxEstimator.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef PERFECTETXESTIMATOR_H_
#define PERFECTETXESTIMATOR_H_

#include <LinkEstimatorBase.h>

class PerfectEtxEstimator: public LinkEstimatorBase {
public:
    PerfectEtxEstimator();
    virtual ~PerfectEtxEstimator();

    // The link cost is 1 for hop count
    virtual double getLinkCost(IPv6Address node);

    virtual double setPacketLoss(IPv6Address node, double packetLoss) {}  // Perfect link estimator does not use link information
};

#endif /* PERFECTETXESTIMATOR_H_ */
