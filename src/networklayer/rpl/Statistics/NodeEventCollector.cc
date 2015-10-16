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

#include "RplDefs.h"
#include "UDPSink.h"
#include "UDPBasicApp.h"

Define_Module(NodeEventCollector);

NodeEventCollector::NodeEventCollector() {
    joined = false;
    isRoot = false;

    numRecvDios = 0;
    numSentDios = 0;
    numSuppressedDios = 0;
    curInt = 0;
    numIntReset = 0;
    numInt = 0;
    numIntDoubled = 0;
    lastPreferredChanged = 0;
    numPreferredChanged = 0;
    curRank = 0;
    curCost = 0;
    curPreferred = INVALID_PARENT;
    numGlobalReset = 0;

    recvPkt = 0;
    sentPkt = 0;
    pktLossRatio = 0;
}

NodeEventCollector::~NodeEventCollector() {
    // TODO Auto-generated destructor stub
}

void NodeEventCollector::initialize(int stage){
    if(stage==0){
        // Local variable initialization
        start = false;

        // Get params
        timeline = par("timeline").boolValue();
        periodic = par("periodic").boolValue();
        period = par("period").doubleValue();
        outDir = par("directory").stringValue();

    } else if( stage == 1 ){
        // Register signals
        rankSignal = registerSignal("rank");
        shortestCostSignal = registerSignal("shortestCost");
        minHopsSignal = registerSignal("minHops");

        gc = check_and_cast<GlobalEventCollector *> (getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("globalStats"));

    } else if( stage == 4 ){
        // It is assumed that the global collector has been initialized

        emit(shortestCostSignal, shortestCost);
        emit(minHopsSignal, minHops);

        // Get app pointer

        // Get sink
        sink = gc->getSink();

        // Get my app
        if( !isRoot && getParentModule()->getParentModule()->getParentModule()->findSubmodule("udpApp", 0) != -1 ){
            app = check_and_cast<UDPBasicApp *> (getParentModule()->getParentModule()->getParentModule()->getSubmodule("udpApp", 0));
        } else {
            // I don't have an app
            app = NULL;
        }

    }

    EventCollector::initialize(stage);
}

void NodeEventCollector::intervalBegin(double i){

    if(curInt != 0){ // This is not the first interval
        if( (curInt * 2) == i ){
            // I has been doubled
            numIntDoubled++;
        } else {
            // This is a reset
            numIntReset++;
        }
    }

    curInt = i;
    numInt++;
}

void NodeEventCollector::messageSuppressed(){
    numSuppressedDios++;
}

void NodeEventCollector::dioSent(DIOmessage* msg){
    numSentDios++;

    traceValue("rplPktSend", numSentDios );
}

void NodeEventCollector::dioReceived(DIOmessage* msg){
    numRecvDios++;

    traceValue("rplPktRecv", numRecvDios);
}

void NodeEventCollector::rankChanged(int newRank){

    curRank = newRank;
    curCost = gc->getPathToRoot(id);

    if(!joined){
        // The node has just joined the network
        joined = true;

        // Signal the Global collector that a node has join the DODAG
        gc->nodeJoined(id);
    }

    traceValue("rank", curRank);
    traceValue("cost", curCost);

}

void NodeEventCollector::preferredParentChanged(int index){
    curPreferred = index;

    lastPreferredChanged = simTime();

    numPreferredChanged++;

    traceValue("preferredParent",index);
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

    numGlobalReset++;
}

void NodeEventCollector::firstDodagStats(){
    logStat("first");
}

void NodeEventCollector::stableDodagStats(){
    logStat("stable");
}

void NodeEventCollector::logStat(std::string status){
    finalValue(status+"_rplRecv", numRecvDios);
    finalValue(status+"_rplSend", numSentDios);
    finalValue(status+"_i_size", curInt);
    finalValue(status+"_i_doubled", numIntDoubled);
    finalValue(status+"_suppressed_dio", numSuppressedDios);

    if(curCost >= 0){ // Negative cost -> disconnected network

        EV << "[STAT] Update stretch cur cost " << curCost << " shortest " << shortestCost << endl;

        finalValue(status+"_routing_shortest_stretch", curCost - shortestCost);

    } else {
        // TODO I should raise some error for disconnected network
    }

    // App stats

    if(!isRoot && app != NULL){
        // If I have an app evaluate stats

        // Recv data
        recvPkt = sink->getNumPktRecv(id);
        // Sent data
        sentPkt = app->getSentPkt();
        // Loss ratio
        pktLossRatio = 1 - ( (double) recvPkt / sentPkt );

        EV << "[STAT] APP " << id << " pkt sent " << sentPkt <<  " pkt recv " << recvPkt << " pkt loss " <<  pktLossRatio << endl;

        finalValue(status+"_app_pkt_loss", pktLossRatio);
    }



}

void NodeEventCollector::nodeRoot(){
    // Node is root
    isRoot = true;

    // Root creates the DODAG, hence it is considered as part of the network already
    joined = true;
    gc->nodeJoined(id);
}