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

#include <Rpl.h>

TrickleNormal::~TrickleNormal() {
    // Nothing to do here
}

void TrickleNormal::initializeTrickle(){
    // Trickle initialization
    curInt = minInt;
    numMessagesReceived = 0;
    curDoublings = 0;

    // Let's schedule the first message
    double nextMsg = simTime().dbl() + uniform(curInt / 2, curInt);

    // Set the timer
    scheduleMessageAt(nextMsg);

    EV << "TrickleNormal: Initialization at " << simTime() << " completed first interval length " <<  curInt << " next message at " << nextMsg << endl;

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

