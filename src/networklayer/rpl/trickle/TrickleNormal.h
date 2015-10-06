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

// Class implementing the plain trickle

#ifndef TRICKLENORMAL_H_
#define TRICKLENORMAL_H_

#include <trickle/ITrickle.h>


class INET_API TrickleNormal: public cSimpleModule, public ITrickle {

    // Initial interval
    double minInt;

    // Maximum number of doublings
    unsigned int maxDoublings;

    // Current interval value
    double curInt;

    // Redundancy value
    unsigned int redundancy;

    // Current messages received
    unsigned int numMessagesReceived;

    // Current number of doublings
    unsigned int curDoublings;

    ////////// Timers //////////

    cMessage *trickle_interval_timer;
    cMessage *trickle_message_timer;
    cMessage *trickle_init_timer;

    // Schedule message at
    void scheduleMessageAt(double t);

    // Schedule interval end at
    void scheduleIntervalAt(double t);

    // Cancel all the scheduled timers
    void cancelAllTimers();

    ////////// Internal interfaces //////////

    // Self-called when the interval ends
    void intervalEnded();

    // Self-called when message timer fires
    void messageFired();

    // Handle self-messages for timing
    void handleMessage(cMessage *msg);

    // Initialize timers
    void initialize(int stage);

    virtual int numInitStages()const { return 4;}

    // Called when trickle is initialized
    virtual void initializeTrickle();

public:
    TrickleNormal() ;

    virtual ~TrickleNormal();

    virtual void reset();

    virtual void dioReceived();
};

#endif /* TRICKLENORMAL_H_ */
