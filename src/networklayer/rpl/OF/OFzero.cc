/*
 * OFzero.cc
 *
 *  Created on: Oct 8, 2015
 *      Author: carlo
 */

#include <OFzero.h>

OFzero::OFzero(bool isRoot, LinkEstimatorBase* l, EventCollector* e) : OFBase(isRoot,l,e){
    max_parent_size = 1; // Only one parent for OF0
    parent_set.resize(1);
    preferred_parent = NULL;
}

OFzero::~OFzero() {
    // TODO Auto-generated destructor stub
}

