/*
 * EventCollections.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef NODEEVENTCOLLECTIONS_H_
#define NODEEVENTCOLLECTIONS_H_

#include "EventCollector.h"
#include "Coord.h"

// Class for NODE event collector

class DIOmessage;
class GlobalEventCollector;

class INET_API NodeEventCollector : public EventCollector {

    friend class GlobalEventCollector;

    GlobalEventCollector* gc;

    //////////// Signals /////////////

    simsignal_t rankSignal;
    simsignal_t shortestCostSignal;
    simsignal_t minHopsSignal;

    //////////// Stats Data Structures /////////////

    double shortestCost;
    int minHops;

    bool joined;

    unsigned int numRecvDios;
    unsigned int numSentDios;

public:
    NodeEventCollector();
    virtual ~NodeEventCollector();

    void initialize(int stage);
    virtual int numInitStages()const { return 5; }

    //////////// Events to be collected /////////////

    // DIOmessage sent
    void dioSent(DIOmessage* msg);

    // DIOmessage received
    void dioReceived(DIOmessage* msg);

    // Rank changed
    void rankChanged(int newRank, double cost);

    // PP changed
    void preferredParentChanged(int index);

    // Global reset
    void globalReset();

};

#endif /* EVENTCOLLECTIONS_H_ */
