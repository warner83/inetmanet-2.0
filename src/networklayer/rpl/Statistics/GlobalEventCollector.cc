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

Define_Module(GlobalEventCollector);

GlobalEventCollector::GlobalEventCollector() {
    topo.extractByProperty("node");
}

GlobalEventCollector::~GlobalEventCollector() {
    // TODO Auto-generated destructor stub
}

void GlobalEventCollector::initialize(int stage){
    if( stage == 3 ){
        // Register signals
        avgRankSignal = registerSignal("avgRank");
        numNodesSignal = registerSignal("numNodes");

        // Get num nodes
        numNodes = topo.getNumNodes();

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

    EventCollector::initializeChannels(stage);
}

void GlobalEventCollector::finish(){

    // Num nodes
    emit(numNodesSignal, numNodes);

    // Average rank
    double avgRank = 0;

    for(int i = 0; i < numNodes; ++i){
        avgRank += rplEngines[i]->rank;
    }

    EV << "[STAT] avgRank " << avgRank/numNodes << endl;

    emit(avgRankSignal, avgRank/numNodes);
}
