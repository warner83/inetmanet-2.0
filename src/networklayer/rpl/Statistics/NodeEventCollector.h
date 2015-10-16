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
class UDPSink;
class UDPBasicApp;

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

    bool isRoot;

    bool joined;

    unsigned int numRecvDios;
    unsigned int numSentDios;
    unsigned int numSuppressedDios;
    double curInt;
    unsigned int numIntDoubled;
    unsigned int numIntReset;
    unsigned int numInt;
    simtime_t lastPreferredChanged;
    unsigned int numPreferredChanged;
    int curPreferred;
    unsigned int curRank;
    double curCost;
    unsigned int numGlobalReset;

    // Application level stats
    UDPSink* sink;
    UDPBasicApp* app; // TODO change to generic app

    unsigned int recvPkt;
    unsigned int sentPkt;
    double pktLossRatio;

public:
    NodeEventCollector();
    virtual ~NodeEventCollector();

    void initialize(int stage);
    virtual int numInitStages()const { return 5; }

    //////////// Events to be collected /////////////

    // Trickle interval begins
    void intervalBegin(double i);

    // Trickle has suppressed a message
    void messageSuppressed();

    // DIOmessage sent
    void dioSent(DIOmessage* msg);

    // DIOmessage received
    void dioReceived(DIOmessage* msg);

    // Rank changed
    void rankChanged(int newRank);

    // PP changed
    void preferredParentChanged(int index);

    // Global reset
    void globalReset();

    // Dodag complete
    void firstDodagStats();

    // Stable dodag complete
    void stableDodagStats();

    // Log stats
    void logStat(std::string status);

    // Node is root
    void nodeRoot();

};

#endif /* EVENTCOLLECTIONS_H_ */
