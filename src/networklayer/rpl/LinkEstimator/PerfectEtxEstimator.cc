/*
 * PerfectEtxEstimator.cc
 *
 *  Created on: Oct 14, 2015
 *      Author: carlo
 */

#include <PerfectEtxEstimator.h>

#include "RplUtils.h"

#include "StationaryMobility.h"
#include "Ieee802154Phy.h"

#define PKT_SIZE 100

PerfectEtxEstimator::PerfectEtxEstimator() {
    topo.extractByProperty("node");
}

PerfectEtxEstimator::~PerfectEtxEstimator() {
    // TODO Auto-generated destructor stub
}

Coord PerfectEtxEstimator::getCoord(int index){
    cModule *mod = topo.getNode(index)->getModule();

    StationaryMobility* mob = check_and_cast<StationaryMobility *> (mod->getSubmodule("mobility"));

    return mob->getCurrentPosition();
}

double PerfectEtxEstimator::getLinkCost(IPv6Address node) {
    // Get pointer to RPL engine
    cModule* owner = check_and_cast<cModule*> (getOwner());

    // Get src index
    Coord srcCoord = getCoord(addrToIndex(node));

    // Get my position
    StationaryMobility* mob = check_and_cast<StationaryMobility *> (owner->getParentModule()->getParentModule()->getParentModule()->getSubmodule("mobility"));
    Coord dstCoord = mob->getCurrentPosition();

    // Retrieve pointer to Phy!
    Ieee802154Phy* phy = check_and_cast<Ieee802154Phy*> (owner->getParentModule()->getParentModule()->getParentModule()->getSubmodule("wlan")->getSubmodule("phy"));

    // Estimate ETX
    return phy->getEtx(srcCoord, dstCoord, PKT_SIZE );
}
