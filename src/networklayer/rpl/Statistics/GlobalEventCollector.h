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
class UDPSink;

class GlobalEventCollector: public EventCollector {

    //////////// Signals /////////////

    simsignal_t firstAvgRankSignal; // Average node rank
    simsignal_t stableAvgRankSignal; // Average node rank

    simsignal_t numNodesSignal; // Num nodes in the network

    //////////// Pointers to simulation modules /////////////

    // Topology structure
    cTopology topo;

    std::vector<RplEngine*> rplEngines;
    std::vector<NodeEventCollector*> nodeCollectors;
    std::vector<Coord> nodePositions;
    Ieee802154Phy* phy;
    UDPSink* sink;

    //////////// Stats Data Structures /////////////

    int numNodes;
    int numJoinedNodes;
    int rootNode;

    std::vector<std::vector<double> > etx; // Etx for each link, -1 if the link is not formed

    std::vector<std::list<int> > shortest_path; // Shortest path for each node
    std::vector<double > shortest_cost; // Overall cost of the shortest path
    std::vector<int > min_hops; // Minimum number of hops

    //////////// Stats Functions /////////////

    void logStat(std::string status);

    void saveMapToFile(std::string fileName);

    // This function is called to collect data on the first DODAG
    void firstDodagStats();

    // This function is called to collect data on the stable DODAG
    void stableDodagStats();

public:
    GlobalEventCollector();
    virtual ~GlobalEventCollector();

    void initialize(int stage);
    virtual int numInitStages()const { return 4; }

    void finish();

    // Each EC uses this function to signal that a node has joined the DODAG
    void nodeJoined(int id);

    // The EC on the root node uses this function to signal the trigger of a global reset
    void globalReset();

    // The EC on the nodes use this function to recover pointer to the Sink
    UDPSink* getSink();

    // Calculate the current cost to root, the EC of each node uses this function to evaluate the current cost
    double getPathToRoot(int node, std::list<int>& path);
    double getPathToRoot(int node);
};

#endif /* GLOBALEVENTCOLLECTOR_H_ */
