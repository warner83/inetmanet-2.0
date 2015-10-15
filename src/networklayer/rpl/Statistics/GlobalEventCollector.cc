/*
 * GlobalEventCollector.cc
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#include <GlobalEventCollector.h>

#include "RplEngine.h"
#include "NodeEventCollector.h"

Define_Module(GlobalEventCollector);

GlobalEventCollector::GlobalEventCollector() {
    topo.extractByProperty("node");
}

GlobalEventCollector::~GlobalEventCollector() {
    // TODO Auto-generated destructor stub
}

void GlobalEventCollector::initialize(int stage){
    if(stage == 3 ){
        // Register signals
        avgRankSignal = registerSignal("avgRank");
        numNodesSignal = registerSignal("numNodes");

        // Get num nodes
        numNodes = topo.getNumNodes();

        // Get pointers to modules

        rplEngines.resize(numNodes);
        nodeCollectors.resize(numNodes);

        for(int i = 0; i < numNodes; ++i){

            cModule *mod = topo.getNode(i)->getModule();

            RplEngine* rpl = check_and_cast<RplEngine *> (mod->getSubmodule("networkLayer")->getSubmodule("rpl")->getSubmodule("rplEngine"));
            rplEngines[i] = rpl;
            nodeCollectors[i] = rpl->ec;

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
