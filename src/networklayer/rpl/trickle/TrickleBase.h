/*
 * TrickleBase.h
 *
 *  Created on: Oct 5, 2015
 *      Author: carlo
 */

#ifndef TRICKLEBASE_H_
#define TRICKLEBASE_H_

#include <omnetpp.h>

class Rpl;

class TrickleBase : public cSimpleModule {

    cMessage *trickle_interval_timer;
    cMessage *trickle_message_timer;

    // Self-called when the interval ends
    virtual void intervalEnded() = 0;

    // Self-called when message timer fires
    virtual void messageFired() = 0;

    // Handle self-messages for timing
    void handleMessage(cMessage *msg);

protected:

    Rpl* rpl;

    // Schedule message at
    void scheduleMessageAt(double t);

    // Schedule interval end at
    void scheduleIntervalAt(double t);

    // Cancel all the scheduled timers
    void cancelAllTimers();

public:
    TrickleBase(Rpl* r);
    virtual ~TrickleBase();

    // Called when trickle is initialized
    virtual void initializeTrickle() = 0;

    // Called when a reset is needed
    virtual void reset() = 0;

    // Called when a consistant packet is received
    virtual void dioReceived() = 0;


};

#endif /* TRICKLEBASE_H_ */
