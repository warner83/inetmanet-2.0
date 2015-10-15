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
#include "Coord.h"

#include "Ieee802154Phy.h"

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
    std::vector<Coord> nodePositions;
    Ieee802154Phy* phy;

    //////////// Stats Data Structures /////////////

    int numNodes;
    int rootNode;

    std::vector<std::vector<double> > etx; // Etx for each link, -1 if the link is not formed

    std::vector<std::list<int> > shortest_path; // Shortest path for each node
    std::vector<double > shortest_cost; // Overall cost of the shortest path
    std::vector<int > min_hops; // Minimum number of hops


public:
    GlobalEventCollector();
    virtual ~GlobalEventCollector();

    void initialize(int stage);
    virtual int numInitStages()const { return 4; }

    void finish();
};

#endif /* GLOBALEVENTCOLLECTOR_H_ */
