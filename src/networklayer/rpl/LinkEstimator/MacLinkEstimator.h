/*
 * MacLinkEstimator.h
 *
 *  Created on: Oct 26, 2015
 *      Author: carlo
 */

#ifndef MACLINKESTIMATOR_H_
#define MACLINKESTIMATOR_H_

#include <LinkEstimatorBase.h>

// Link estimator based on the real feedback from the MAC layer

// Still TODO

class MacLinkEstimator: public LinkEstimatorBase {
public:
    MacLinkEstimator();
    virtual ~MacLinkEstimator();

    virtual double getLinkCost(IPv6Address node);

    virtual double setPacketLoss(IPv6Address node, double packetLoss);
};

#endif /* MACLINKESTIMATOR_H_ */
