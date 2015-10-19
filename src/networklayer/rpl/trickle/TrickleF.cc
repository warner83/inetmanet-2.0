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

#include <TrickleF.h>

Define_Module(TrickleF);

TrickleF::TrickleF() {
    // Nothing to do here

}

TrickleF::~TrickleF() {
    // Nothing to do here
}

void TrickleF::initialize(int stage)
{
    suppressedDIOs = 0 ;

    EV << "[TrickleF] " << redundancy << endl;

    TrickleNormal::initialize(stage);
}

void TrickleF::intervalEnded(){
    // Double interval, if the maximum number of doublings is not reached
    if( curDoublings < maxDoublings ){
        // Double I
        curInt *= 2;
        curDoublings++;
    }

    // Let's schedule the message (according to trickle-f algorithm this depends on the number of suppressed dios
    double nextMsg = simTime().dbl() + uniform(curInt / (pow(2, suppressedDIOs + 1) ), curInt / (pow(2, suppressedDIOs)));

    // Set the timer
    scheduleMessageAt(nextMsg);

    // Reset number of messages received
    numMessagesReceived = 0;

    // Let's schedule the end of the interval
    double nextInt = simTime().dbl() + curInt ;
    scheduleIntervalAt(nextInt);

    EV << "Trickle: Interval ended at " << simTime() << " new interval length " <<  curInt << " next message at " << nextMsg << endl;
    EV << "TrickleF: firing interval " << curInt / (pow(2, suppressedDIOs + 1) ) << ", " <<  curInt / (pow(2, suppressedDIOs)) << endl;

    // Signal EC
    ec->intervalBegin(curInt);
}

void TrickleF::messageFired() {
    if( numMessagesReceived > redundancy ){
        // I'm going to suppress the broadcast, update the counter
        suppressedDIOs++;

        // Signal EC
        ec->messageSuppressed();
    } else {
        // I'm sending my DIO message, reset the counter
        suppressedDIOs = 0;
    }

    TrickleNormal::messageFired();

}

void TrickleF::initializeTrickle(){
    suppressedDIOs = 0 ;

    TrickleNormal::initializeTrickle();
}
