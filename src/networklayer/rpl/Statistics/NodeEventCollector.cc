/*
 * EventCollections.cc
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#include <NodeEventCollector.h>

#include <sstream>
#include <string>
#include <fstream>


Define_Module(NodeEventCollector);

NodeEventCollector::NodeEventCollector() {
    // TODO Auto-generated constructor stub

}

NodeEventCollector::~NodeEventCollector() {
    // TODO Auto-generated destructor stub
}

void NodeEventCollector::initialize(int stage){
    if(stage == 0 ){
        // Register signals
        rankSignal = registerSignal("rank");
    }

    EventCollector::initialize(stage);
}

void NodeEventCollector::rankChanged(int id, int newRank){
    EV << "[STAT] rankChanged " << newRank << endl;

    emit(rankSignal, newRank);
}
