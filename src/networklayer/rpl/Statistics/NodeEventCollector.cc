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

#include "DIOmessage_m.h"

#include "GlobalEventCollector.h"


Define_Module(NodeEventCollector);

NodeEventCollector::NodeEventCollector() {
    numRecvDios = 0;
    numSentDios = 0;
    joined = false;
    isRoot = false;
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

        gc = check_and_cast<GlobalEventCollector *> (getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("globalStats"));

    } else if( stage == 4 ){
        // It is assumed that the global collector has been initialized

        emit(shortestCostSignal, shortestCost);
        emit(minHopsSignal, minHops);

    }

    EventCollector::initialize(stage);
}

void NodeEventCollector::dioSent(DIOmessage* msg){\

}

void NodeEventCollector::dioReceived(DIOmessage* msg){

}

void NodeEventCollector::rankChanged(int newRank, double cost){

    EV << "[STAT] rankChanged " << newRank << endl;

    if(!joined){
        // The node has just joined the network
        joined = true;

        // Signal the Global collector that a node has join the DODAG
        gc->nodeJoined(id);
    }

    emit(rankSignal, newRank);
}

void NodeEventCollector::preferredParentChanged(int index){

}

void NodeEventCollector::globalReset(){

    if(isRoot){
        // Signal to the Global GC that a global reset has been triggered
        gc->globalReset();

        // Root creates the DODAG, hence it is considered as part of the network already
        gc->nodeJoined(id);
    } else {
        joined = false;
    }
}

void NodeEventCollector::dodagComplete(){

}

void NodeEventCollector::nodeRoot(){
    // Node is root
    isRoot = true;

    // Root creates the DODAG, hence it is considered as part of the network already
    joined = true;
    gc->nodeJoined(id);
}
