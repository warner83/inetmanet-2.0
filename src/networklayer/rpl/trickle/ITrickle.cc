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

void ITrickle::initialize(int stage){
    // Get pointer to EC
    ec = check_and_cast<NodeEventCollector*> (this->getParentModule()->getSubmodule("stats"));
}

void ITrickle::signalEngine(int kind){
    // Create a new message and send it out to the engine
    cMessage* msg = new cMessage("",kind);
    send(msg, "engineOut");
}



