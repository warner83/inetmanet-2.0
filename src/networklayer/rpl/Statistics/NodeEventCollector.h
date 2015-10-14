/*
 * EventCollections.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef NODEEVENTCOLLECTIONS_H_
#define NODEEVENTCOLLECTIONS_H_

#include "EventCollector.h"

// Class for NODE event collector

class INET_API NodeEventCollector : public EventCollector {

    friend class GlobalEventCollector;

    //////////// Signals /////////////

    simsignal_t rankSignal;


public:
    NodeEventCollector();
    virtual ~NodeEventCollector();

    void initialize(int stage);

    //////////// Events to be collected /////////////

    // Rank changed
    void rankChanged(int id, int newRank);

};

#endif /* EVENTCOLLECTIONS_H_ */
