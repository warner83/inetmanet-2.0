/*
 * MRHOF.cc
 *
 *  Created on: Oct 8, 2015
 *      Author: carlo
 */

#include <MRHOF.h>

MRHOF::MRHOF(bool isRoot, LinkEstimatorBase* l , EventCollector* e, int pp_size) : OFBase(isRoot, l, e){
    max_parent_size = pp_size;
    parent_set.resize(pp_size);
    preferred_parent = NULL;
}


MRHOF::~MRHOF() {
    // TODO Auto-generated destructor stub
}

