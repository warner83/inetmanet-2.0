/*
 * OFzero.h
 *
 *  Created on: Oct 8, 2015
 *      Author: carlo
 */

#ifndef OFZERO_H_
#define OFZERO_H_

#include <OFBase.h>

class OFzero: public OFBase {

    double evaluateCost(struct parent* p){
        // Final cost as advRank + link_metric * MIN_HOP_RANK_INCREASE
        p->cost = floor(p->rank + p->linkMetric * MIN_HOP_RANK_INCREASE);
    }

    void manageParentWithLowerRank(struct parent* p){
        if( parent_size > 0 && parent_set[0].cost == p->cost )
            // The rank is the same -> do nothing
            return;

            parent_set[0] = *p;
            parent_size = 1;

            // Set as preferred parent the only parent I've
            preferred_parent = &parent_set[0];
    }

    double evaluateRank(){
        // Node rank is the cost through preferred parent
        double ret = 0;
        if(preferred_parent == NULL)
            ret = INFINITE_RANK;
        else ret = preferred_parent->cost;
        return ret;
    }

    void manageChangeRank(){} // OF0 Do nothing


public:
    OFzero(bool isRoot, LinkEstimatorBase* l);
    virtual ~OFzero();
};

#endif /* OFZERO_H_ */
