/*
 * HopCountEstimator.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef HOPCOUNTESTIMATOR_H_
#define HOPCOUNTESTIMATOR_H_

#include <LinkEstimatorBase.h>

// Hop Count Estimator, the cost is 1 for each link

class HopCountEstimator: public LinkEstimatorBase {
public:
    HopCountEstimator();
    virtual ~HopCountEstimator();

    // The link cost is 1 for hop count
    virtual double getLinkCost(IPv6Address node) {return 1;}

    virtual double setPacketLoss(IPv6Address node, double packetLoss) {} // Hop count does not use link information
};

#endif /* HOPCOUNTESTIMATOR_H_ */
