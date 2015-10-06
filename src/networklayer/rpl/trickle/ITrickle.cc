/*
 * TrickleBase.cc
 *
 *  Created on: Oct 5, 2015
 *      Author: carlo
 */

#include <trickle/ITrickle.h>


ITrickle::ITrickle() {
}

ITrickle::~ITrickle() {
}

void ITrickle::setRplPtr(RplEngine* r){
    rpl = r;
}



