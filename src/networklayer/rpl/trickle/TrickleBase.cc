/*
 * TrickleBase.cc
 *
 *  Created on: Oct 5, 2015
 *      Author: carlo
 */

#include <trickle/TrickleBase.h>

#include <Rpl.h>

#define trickle_interval_timer_kind_self_message         1
#define trickle_message_timer_kind_self_message             2

TrickleBase::TrickleBase(Rpl* r) {
    rpl = r;

    trickle_interval_timer = new cMessage();
    trickle_interval_timer->setKind(trickle_interval_timer_kind_self_message);

    trickle_message_timer = new cMessage();
    trickle_message_timer->setKind(trickle_message_timer_kind_self_message);
}

TrickleBase::~TrickleBase() {
    delete trickle_interval_timer;
    delete trickle_message_timer;
}


void TrickleBase::scheduleMessageAt(double t){
    scheduleAt(t, trickle_message_timer);
}

void TrickleBase::scheduleIntervalAt(double t){
    scheduleAt(t, trickle_interval_timer);
}

void TrickleBase::cancelAllTimers(){
    cancelEvent(trickle_interval_timer);
    cancelEvent(trickle_message_timer);
}

void TrickleBase::handleMessage(cMessage *msg)
{
    if (!msg->isSelfMessage()){
        error("Not self message within trickle");
    } else {
        if(msg->getKind()==trickle_interval_timer_kind_self_message){
            // Interval ended
            intervalEnded();
        } else if(msg->getKind()==trickle_message_timer_kind_self_message){
            // Time to send (?)
            messageFired();
        }
    }
}


