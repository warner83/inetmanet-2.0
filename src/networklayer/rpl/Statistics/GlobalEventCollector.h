/*
 * GlobalEventCollector.h
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#ifndef GLOBALEVENTCOLLECTOR_H_
#define GLOBALEVENTCOLLECTOR_H_

#include <EventCollector.h>

#include <vector>

class RplEngine;
class NodeEventCollector;

class GlobalEventCollector: public EventCollector {

    //////////// Signals /////////////

    simsignal_t avgRankSignal; // Average node rank
    simsignal_t numNodesSignal; // Num nodes in the network

    //////////// Pointers to simulation modules /////////////

    // Topology structure
    cTopology topo;

    std::vector<RplEngine*> rplEngines;
    std::vector<NodeEventCollector*> nodeCollectors;

    //////////// Stats Data Structures /////////////

    int numNodes;

public:
    GlobalEventCollector();
    virtual ~GlobalEventCollector();

    void initialize(int stage);
    virtual int numInitStages()const { return 4; }

    void finish();
};

#endif /* GLOBALEVENTCOLLECTOR_H_ */
