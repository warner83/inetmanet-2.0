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
        shortestCostSignal = registerSignal("shortestCost");
        minHopsSignal = registerSignal("minHops");
    } else if( stage == 4 ){
        // It is assumed that the global collector has been initialized

        emit(shortestCostSignal, shortestCost);
        emit(minHopsSignal, minHops);

    }

    EventCollector::initialize(stage);
}

void NodeEventCollector::rankChanged(int id, int newRank){
    EV << "[STAT] rankChanged " << newRank << endl;

    emit(rankSignal, newRank);
}
