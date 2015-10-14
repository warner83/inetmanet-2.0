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

        // Get pointers to module

        rplEngines.resize(topo.getNumNodes());
        nodeCollectors.resize(topo.getNumNodes());

        for(int i = 0; i < topo.getNumNodes(); ++i){

            cModule *mod = topo.getNode(i)->getModule();

            RplEngine* rpl = check_and_cast<RplEngine *> (mod->getSubmodule("networkLayer")->getSubmodule("rpl"));
            rplEngines[i] = rpl;
            nodeCollectors[i] = rpl->ec;

        }
    }

    EventCollector::initializeChannels(stage);
}

void GlobalEventCollector::finish(){
    double avgRank = 0;

    for(int i = 0; i < topo.getNumNodes(); ++i){
        avgRank += rplEngines[i]->rank;
    }

    EV << "[STAT] avgRank " << avgRank << endl;

    emit(avgRankSignal, avgRank);
}
