/*
 * GlobalEventCollector.cc
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#include <GlobalEventCollector.h>

#include "RplEngine.h"
#include "NodeEventCollector.h"
#include "StationaryMobility.h"
#include "RplDefs.h"
#include "UDPSink.h"

Define_Module(GlobalEventCollector);

GlobalEventCollector::GlobalEventCollector() {
    topo.extractByProperty("node");

    numJoinedNodes = 0;
}

GlobalEventCollector::~GlobalEventCollector() {
    // TODO Auto-generated destructor stub
}

double GlobalEventCollector::getPathToRoot(int node, std::list<int>& path){
    double cost = 0;

    int actual = node;
    path.push_back(actual);
    while( actual != rootNode ){
        if(actual == -1){
            // The node is still disconnected
            return -1;
        }

        int next = nodeCollectors[actual]->curPreferred;
        double c = etx[actual][next];

        cost += c;
        actual = next;

        path.push_back(actual);
    }

    return cost;
}

double GlobalEventCollector::getPathToRoot(int node){
    std::list<int> path;

    double cost = getPathToRoot(node, path);

    return cost;
}

UDPSink* GlobalEventCollector::getSink(){
    return sink;
}

void GlobalEventCollector::initialize(int stage){
    if( stage == 3 ){
        // Register signals
        firstAvgRankSignal = registerSignal("firstAvgRank");
        stableAvgRankSignal = registerSignal("stableAvgRank");

        numNodesSignal = registerSignal("numNodes");

        // Get num nodes
        numNodes = topo.getNumNodes();
        emit(numNodesSignal, numNodes);

        // Get pointers to modules

        rplEngines.resize(numNodes);
        nodeCollectors.resize(numNodes);
        nodePositions.resize(numNodes);

        for(int i = 0; i < numNodes; ++i){

            cModule *mod = topo.getNode(i)->getModule();

            RplEngine* rpl = check_and_cast<RplEngine *> (mod->getSubmodule("networkLayer")->getSubmodule("rpl")->getSubmodule("rplEngine"));
            rplEngines[i] = rpl;
            nodeCollectors[i] = rpl->ec;

            if(rplEngines[i]->isRoot)
                rootNode = i;

            // Get nodes position
            StationaryMobility* mob = check_and_cast<StationaryMobility *> (mod->getSubmodule("mobility"));
            nodePositions[i] = mob->getCurrentPosition();
        }

        // Get pointer to a phy module, for etx estimation, any phy will work, I pick the phy of the first node (hey you're gonna have at least one node in the network)
        phy = check_and_cast<Ieee802154Phy*> (topo.getNode(0)->getModule()->getSubmodule("wlan")->getSubmodule("phy"));

        // Get topology information

        // Collect ETX information
        etx.resize(numNodes);

        for (int i=0; i<numNodes; i++)
        {

            etx[i].resize(numNodes);

            for (int j=0; j<numNodes; j++)
            {

                if( i == j )
                    // i and j are the same node
                    continue;

                // Get ETX
                double etx_link = phy->getEtx(nodePositions[i], nodePositions[j], PKT_SIZE_ETX_ESTIMATION );

                // Check ETX rank
                if( etx_link >= 0 && etx_link < MAX_ETX ){

                    etx[i][j]=etx_link;

                } else {
                    etx[i][j]=-1;
                }
            }
        }

        // Evaluate shortest paths

        shortest_cost.resize(numNodes);
        shortest_path.resize(numNodes);
        min_hops.resize(numNodes);

        // Implement Dijkstra alg

        #define INFINITY 10000000000
        #define UNDEF -1

        std::vector<double> dist;
        dist.reserve(numNodes);
        std::vector<int> prev;
        prev.reserve(numNodes);
        std::map<int, double> Q;
        for (int i=0; i<numNodes; i++){
            dist[i]=INFINITY;
            prev[i]=UNDEF;
            Q[i] = INFINITY;
        }

        dist[rootNode] = 0;
        Q[rootNode] = 0;

        while( Q.size() > 0 ){
            // Find smallest distance
            int min;
            double min_cost = INFINITY;
            std::map<int, double>::iterator it;
            for( it = Q.begin(); it != Q.end(); ++it ){
                if( min_cost > (*it).second ){
                    // New min
                    min = (*it).first;
                    min_cost = (*it).second;
                }
            }

           if( min_cost == INFINITY )
                opp_error("Disconnected network");

           Q.erase(min);

           for( int n = 0; n < numNodes; ++n ){

               if( Q.find(n) == Q.end() || etx[min][n] == -1 )
                   continue;

               double new_dist = dist[min] + etx[min][n];

               if( new_dist < dist[n] ){
                   dist[n] = new_dist;
                   prev[n] = min;
                   Q[n] = new_dist;
               }
           }
        }

        // Collect result
        EV << "[STAT] Shortest paths:" << endl;
        for (int i=0; i< numNodes; i++){
            shortest_cost[i] = dist[i];

            // Set the cost to the node event collector
            nodeCollectors[i]->shortestCost = shortest_cost[i];

            EV << i << " " << shortest_cost[i] << " : ";

            int actual = i;
            EV << actual << " ";
            shortest_path[i].push_back(actual);
            while(actual != rootNode){
                actual = prev[actual];
                shortest_path[i].push_back(actual);

                EV << actual << " ";
            }
            EV << std::endl;

            min_hops[i] = shortest_path[i].size();
            nodeCollectors[i]->minHops = min_hops[i];

        }


    }

    // Get pointer to UDPSink
    cModule *mod = topo.getNode(rootNode)->getModule();
    sink = check_and_cast<UDPSink *> (mod->getSubmodule("udpApp", 0));

    EventCollector::initializeChannels(stage);
}

void GlobalEventCollector::finish(){
    // Collect statistics on the stable DODAG
    stableDodagStats();
}

void GlobalEventCollector::nodeJoined(int id){
    numJoinedNodes++;
    if(numJoinedNodes == numNodes){
        // All nodes in the network has joined the DODAG, let count some stats
        EV << "[STAT] All nodes have joined the DODAG" << endl;

        // Signal to all the nodes that the DODAG is now completed
        for(int i = 0; i < numNodes; ++i){
            nodeCollectors[i]->firstDodagStats();
        }

        // Collect statistics on the first DODAG
        firstDodagStats();
    }
}

void GlobalEventCollector::globalReset(){
    stableDodagStats();

}

void GlobalEventCollector::firstDodagStats(){
    logStat("first");
}

void GlobalEventCollector::stableDodagStats(){
    // Check if the dodag is formed
    if(numJoinedNodes == numNodes){

        // Signal to all the nodes that the DODAG is stable
        for(int i = 0; i < numNodes; ++i){
            nodeCollectors[i]->stableDodagStats();
        }

        // Collect statistics on the stable DODAG
        logStat("stable");
    }
}

void GlobalEventCollector::logStat(std::string status){

    // Find all the max values
    simtime_t max_stable = 0; // The stable time is the time when the last PP change happened in the network
    unsigned int max_rank = 0;
    double max_cost = 0;
    for(int i = 0; i < numNodes; ++i){
        if( max_stable < nodeCollectors[i]->lastPreferredChanged ){
            max_stable = nodeCollectors[i]->lastPreferredChanged;
        }

        if( max_rank < nodeCollectors[i]->curRank){
            max_rank = nodeCollectors[i]->curRank;
        }

        if( max_cost < nodeCollectors[i]->curCost){
            max_cost = nodeCollectors[i]->curCost;
        }

    }

    if(status.compare("first") == 0)
        finalValue("first_time", simTime().dbl() );

    if(status.compare("stable") == 0)
        finalValue("stable_time", max_stable.dbl() );

    finalValue(status+"_totalMaxRank", max_rank);
    finalValue(status+"_totalMaxCost", max_cost);


    // Get totals from all the nodes

    unsigned int total_rpl_recv = 0;
    unsigned int total_rpl_send = 0;
    unsigned int parent_changed = 0;
    double overall_stretch = 0;
    unsigned int overall_suppressed = 0;

    for(int i = 0; i < numNodes; ++i){
        total_rpl_recv += nodeCollectors[i]->numRecvDios;
        total_rpl_send += nodeCollectors[i]->numSentDios;
        parent_changed += nodeCollectors[i]->numPreferredChanged;
        overall_suppressed += nodeCollectors[i]->numSuppressedDios;
        overall_stretch += nodeCollectors[i]->curCost - nodeCollectors[i]->shortestCost;
    }

    finalValue(status+"_totalRplRcv", total_rpl_recv);
    finalValue(status+"_totalRplSend", total_rpl_send);
    finalValue(status+"_total_parent_changed", parent_changed);
    finalValue(status+"_overall_routing_shortest_stretch", overall_stretch);
    finalValue(status+"_overall_suppressed_dio", overall_suppressed);

    // Get all the average values
    double avg_rank = 0;

    for(int i = 0; i < numNodes; ++i){
        avg_rank += rplEngines[i]->rank;
    }

    avg_rank /= numNodes;

    finalValue(status+"_totalAvgRank", avg_rank);

    // Evaluate app data

    unsigned int totalRecvPkt = 0;
    unsigned int totalSentPkt = 0;
    double totalPktLossRatio = 0;

    for(int i = 0; i < numNodes; ++i){
        if(i == rootNode)
            continue;

        if( nodeCollectors[i]->app == NULL )
            continue; // This node does not have an app

        // Recv data
        totalRecvPkt += nodeCollectors[i]->recvPkt;
        // Sent data
        totalSentPkt += nodeCollectors[i]->sentPkt;
    }

    // Loss ratio
    totalPktLossRatio = 1 - ( (double) totalRecvPkt / totalSentPkt );

    EV << "[STAT] Overall app performance " << " pkt sent " << totalSentPkt <<  " pkt recv " << totalRecvPkt << " pkt loss " <<  totalPktLossRatio << endl;

    finalValue(status+"_total_pkt_loss", totalPktLossRatio);


    //emit(stableAvgRankSignal, avgRank);
    //emit(firstAvgRankSignal, avgRank);

}

