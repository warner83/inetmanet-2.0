//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <trickle/TrickleNormal.h>

#include <RplEngine.h>

#define trickle_interval_timer_kind_self_message         1
#define trickle_message_timer_kind_self_message             2
#define trickle_init_timer_kind_self_message            3

Define_Module(TrickleNormal);

TrickleNormal::TrickleNormal()  {
    // Nothing to do here
}

TrickleNormal::~TrickleNormal() {
    // Nothing to do here
}

void TrickleNormal::initialize(int stage)
{
    if(stage == 0){
        trickle_interval_timer = new cMessage();
        trickle_interval_timer->setKind(trickle_interval_timer_kind_self_message);

        trickle_message_timer = new cMessage();
        trickle_message_timer->setKind(trickle_message_timer_kind_self_message);

        minInt = par("minInt").doubleValue();
        maxDoublings = par("maxDoublings").doubleValue();
        redundancy = par("redundancy").doubleValue();

        // Set timer for trickle initialization
        simtime_t initAbsTime = simTime() + par("initTime").doubleValue();

        // Initialize the timer
        trickle_init_timer = new cMessage();
        trickle_init_timer->setKind(trickle_init_timer_kind_self_message);
        scheduleAt(initAbsTime, trickle_init_timer);

        EV << "Trickle: Trickle will start at " << initAbsTime << endl;
    }
}

void TrickleNormal::scheduleMessageAt(double t){
    //take(trickle_message_timer);
    scheduleAt(t, trickle_message_timer);
}

void TrickleNormal::scheduleIntervalAt(double t){
    //take(trickle_interval_timer);
    scheduleAt(t, trickle_interval_timer);
}

void TrickleNormal::cancelAllTimers(){
    //take(trickle_message_timer);
    //take(trickle_interval_timer);
    cancelEvent(trickle_interval_timer);
    cancelEvent(trickle_message_timer);
}

void TrickleNormal::handleMessage(cMessage *msg)
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
        } else if (msg->getKind()==trickle_init_timer_kind_self_message) {
            initializeTrickle();
        }
    }
}


void TrickleNormal::initializeTrickle(){

    // Trickle initialization
    curInt = minInt;
    numMessagesReceived = 0;
    curDoublings = 0;

    // Let's schedule the first message
    double nextMsg = simTime().dbl() + uniform(curInt / 2, curInt);

    EV << "TrickleNormal: Initialization at " << simTime() << " completed first interval length " <<  curInt << " next message at " << nextMsg << endl;

    // Set the timer
    scheduleMessageAt(nextMsg);

}

void TrickleNormal::reset(){

    EV << "TrickleNormal: Reset!" << endl;

    // Cancel all pending actions
    cancelAllTimers();

    // Initialize trickle again
    initializeTrickle();
}

void TrickleNormal::dioReceived(){
    numMessagesReceived++;
}

void TrickleNormal::intervalEnded(){
    // Double interval, if the maximum number of doublings is not reached
    if( curDoublings < maxDoublings ){
        // Double I
        curInt *= 2;
        curDoublings++;
    }

    // Let's schedule the message
    double nextMsg = simTime().dbl() + uniform(curInt / 2, curInt);

    // Set the timer
    scheduleMessageAt(nextMsg);

    // Reset number of messages received
    numMessagesReceived = 0;

    EV << "TrickleNormal: Interval ended at " << simTime() << " new interval length " <<  curInt << " next message at " << nextMsg << endl;
}

void TrickleNormal::messageFired() {
    if( numMessagesReceived < redundancy ){
        EV << "TrickleNormal: Sending DIO out received messages " << numMessagesReceived << endl;

        // Send out a DIO message only if the number of messages received is below the redundancy threshold
        rpl->sendDioOut();
    } else {
        EV << "TrickleNormal: DIO suppressed " << numMessagesReceived << endl;
    }
}

