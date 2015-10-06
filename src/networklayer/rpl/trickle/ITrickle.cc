/*
 * TrickleBase.cc
 *
 *  Created on: Oct 5, 2015
 *      Author: carlo
 */

#include <trickle/ITrickle.h>

Register_Abstract_Class(ITrickle);

ITrickle::ITrickle() {
}

ITrickle::~ITrickle() {
}

void ITrickle::setRplPtr(RplEngine* r){
    rpl = r;
}



