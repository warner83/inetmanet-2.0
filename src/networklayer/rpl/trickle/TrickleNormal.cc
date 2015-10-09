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

#include <trickle/TrickleEvents.h>

// Self timing messages
#define trickle_interval_timer_kind_self_message         1
#define trickle_message_timer_kind_self_message             2

Define_Module(TrickleNormal);

TrickleNormal::TrickleNormal()  {
    EV << "[TRICKLE] Node configured with trickle normal! " << endl;
}

TrickleNormal::~TrickleNormal() {
    cancelAndDelete(trickle_interval_timer);
    cancelAndDelete(trickle_message_timer);
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

    }
}

void TrickleNormal::scheduleMessageAt(double t){
    cancelEvent(trickle_message_timer);
    scheduleAt(t, trickle_message_timer);
}

void TrickleNormal::scheduleIntervalAt(double t){
    cancelEvent(trickle_interval_timer);
    scheduleAt(t, trickle_interval_timer);
}

void TrickleNormal::cancelAllTimers(){
    cancelEvent(trickle_interval_timer);
    cancelEvent(trickle_message_timer);
}

void TrickleNormal::handleMessage(cMessage *msg)
{
    if (!msg->isSelfMessage()){
        // The message is from the rpl engine
        if(msg->getKind()==message_received){
            dioReceived();
        } else if(msg->getKind()==rpl_reset){
            reset();
        } else if(msg->getKind()==rpl_init){
            initializeTrickle();
        }

        // Delete only messages from other modules
        delete msg;
    } else {
        // Self message for internal timer
        if(msg->getKind()==trickle_interval_timer_kind_self_message){
            // Interval ended
            intervalEnded();
        } else if(msg->getKind()==trickle_message_timer_kind_self_message){
            // Time to send (?)
            messageFired();
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

    // Let's schedule the end of the interval
    double nextInt = simTime().dbl() + curInt ;

    EV << "[TRICKLE] Initialization at " << simTime() << " first interval length " <<  curInt << " next message at " << nextMsg << endl;

    // Set the timer
    scheduleMessageAt(nextMsg);
    scheduleIntervalAt(nextInt);

}

void TrickleNormal::reset(){

    EV << "[TRICKLE] Reset!" << endl;

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

    // Let's schedule the end of the interval
    double nextInt = simTime().dbl() + curInt ;
    scheduleIntervalAt(nextInt);

    EV << "[TRICKLE] Interval ended at " << simTime() << " new interval length " <<  curInt << " next message at " << nextMsg << endl;
}

void TrickleNormal::messageFired() {
    if( numMessagesReceived <= redundancy ){
        EV << "[TRICKLE] Sending DIO out received messages " << numMessagesReceived << endl;

        // Send out a DIO message only if the number of messages received is below the redundancy threshold
        signalEngine(send_dio_message);
    } else {
        EV << "[TRICKLE] DIO suppressed " << numMessagesReceived << endl;
    }
}

