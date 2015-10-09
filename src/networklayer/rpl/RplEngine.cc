//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <RplEngine.h>

#include "RPLmessage_m.h"
#include "DIOmessage_m.h"

#include "RplDefs.h"

#include "trickle/TrickleEvents.h"
#include "OF/OFzero.h"
#include "OF/MRHOF.h"

#include "ICMPv6Message_m.h"
#include "IPv6Datagram.h"

// Timer definitions, used only by root nodes
#define init_timer_kind_self_message        1
#define reset_timer_kind_self_message        2

Define_Module(RplEngine);

RplEngine::RplEngine() {
    // Nothing to do here
}

RplEngine::~RplEngine() {
    cancelAndDelete(init_timer);
    cancelAndDelete(reset_timer);

    delete of;
}

void RplEngine::initialize(int stage)
{
    if(stage == 1){

        // Recover params
        isRoot = par("isRoot").boolValue();

        // Instantiate the Objective function
        if(strcmp(par("objectiveFunctionType").stringValue(), "OFzero") == 0){
            of = (OFBase*) new OFzero(isRoot);
        } else if(strcmp(par("objectiveFunctionType").stringValue(), "MRHOF") == 0 ){
            of = (OFBase*) new MRHOF(isRoot, PREFERRED_PARENT_SIZE); // TODO set preferred parent size as param
        } else {
            // OF invalid
            std::cout << "[RPL] unknown objective function " << par("objectiveFunctionType").str() << endl;
            abort();
        }

    } else if(stage == 3){

        // Recover system pointers and my address
        ift = InterfaceTableAccess().get(); // Interface table
        ie = ift->getInterface(WPAN_INTERFACE); // WPAN interface entry
        myIp = ie->ipv6Data()->getLinkLocalAddress();

        EV <<"[RPL] My IPv6 is " << myIp.str() << endl;

        // Initialize RPL engine
        engineInitialize();

    }
}

void RplEngine::engineInitialize(){
    if(isRoot){
        // Root initialization

        // Root initializes itself
        initialized = true;

        // As root I set the DODAGID as my IP
        dodagID = myIp;

        // Root set rank as MIN_HOP_RANK_INCREASE
        rank = MIN_HOP_RANK_INCREASE;

        // Set DODAG version to the first valid
        dodagVersion = 1;

        EV << "[RPL] ROOT node creates DODAG " << dodagID << " with rank " << rank << endl;

        // If root set timer for initialization
        simtime_t initAbsTime = simTime() + par("initTime").doubleValue();

        // Initialize the init timer
        init_timer = new cMessage();
        init_timer->setKind(init_timer_kind_self_message);
        // Schedule trickle initialization
        scheduleAt(initAbsTime, init_timer);

        EV << "[RPL] Trickle will start at " << initAbsTime << endl;
    } else {
        // Non-root initialization

        // RPL will be inactive until the first DIO is received
        initialized = false;

        // Set my rank to infinite
        rank = INFINITE_RANK;

        // Set DODAG version to an invalid value
        dodagVersion = INVALID_DODAG_VERSION;

        // For non-root nodes initialize the dodag as unspecified address
        dodagID = IPv6Address::UNSPECIFIED_ADDRESS;
    }

    // Set an invalid preferred parent
    preferredParent = IPv6Address::UNSPECIFIED_ADDRESS;
}

void RplEngine::handleMessage(cMessage *msg)
{

    if (!msg->isSelfMessage())
    {
        if( strcmp(msg->getArrivalGate()->getFullName(), "ipv6In" ) == 0 ){
            // Message form the network

            EV<<"[RPL] RPL message received\n";

            RPLmessage *rplMessage = check_and_cast<RPLmessage *>(msg);

            if(rplMessage->getCode() == DIO){
                DIOmessage *dioMessage = check_and_cast<DIOmessage *>(msg);
                EV<<"[RPL] DIO message received from " << rplMessage->getSrc().str() << " DODAGID " << dioMessage->getDODAGID().str() << " rank " << dioMessage->getRank() << endl;

                // Signal trickle that a message has been received
                signalTrickle(message_received);

                if( !isRoot ){
                    // Root node ignores DIO messages!

                    if( dodagID.compare(IPv6Address::UNSPECIFIED_ADDRESS) == 0 ){
                        // The node still has not joined any DODAG

                        // NOTE policy of this implementation is to join the DODAG from which the first message is received

                        // Let's join this DODAG
                        dodagID = dioMessage->getDODAGID();
                        dodagVersion = dioMessage->getVersionNumber();

                        // Reset OF data, this will cause an inconsistency
                        of->reset();

                        EV << "[RPL] Join DODAG " << dioMessage->getDODAGID() << endl;

                    }

                    if ( dioMessage->getDODAGID().compare(dodagID) == 0 ) {
                        // The message belongs to my DODAG

                        EV << "[RPL] My status is DODAGID " << dodagID.str() << " DODAG VERSION " << dodagVersion << " rank " << rank << " preferred parent " << preferredParent.str() << endl;

                        // Check the DODAG version number
                        if( dodagVersion < dioMessage->getVersionNumber() ){

                            // The version has changed! This is a global reset!

                            // Update version
                            dodagVersion = dioMessage->getVersionNumber();

                            // Reset rank
                            rank = INFINITE_RANK;

                            // Reset OF data, this will cause an inconsistency
                            of->reset();

                            EV << "[RPL] Global reset, new DODAG version " << dodagVersion << endl;

                        }

                        // Let OF process the DIO message for rank evaluation and parent selection
                        bool inconsistency = of->process_dio( rplMessage->getSrc(), dioMessage );

                        if(inconsistency){
                            // DIO message has triggered some changes (e.g. preferred parent, rank or global reset)

                            // Reset trickle
                            signalTrickle(rpl_reset);

                            // Update rank
                            rank = of->getRank();

                            // Set the new preferred parent
                            preferredParent = of->getPP();

                            EV << "[RPL] Inconsistency detected, DODAGID " << dodagID.str() << " DODAG VERSION " << dodagVersion << " rank " << rank << " preferred parent " << preferredParent.str() << endl;

                        } else {
                            EV << "[RPL] Rank " << rank << " confirmed" << endl;
                        }

                    } else {
                        // Do nothing, the message belongs to a different DODAG
                        EV << "[RPL] Ignoring DIO message from a different DODAG " << dioMessage->getDODAGID() << endl;
                    }

                    if( !initialized ){
                        // This is my first DIO message
                        initialized = true;

                        // Let's wake trickle up and start advertise myself!
                        signalTrickle(rpl_init);

                    }
                }

            } else {
                EV << "[RPL] unknown rpl message from network " << rplMessage->getCode() << endl;
                abort();
            }
        } else if( strcmp(msg->getArrivalGate()->getFullName(), "trickleIn" ) == 0 ){
            // Internal message from trickle
            if( msg->getKind() == send_dio_message ){
                // Fire a DIO message
                sendDioOut();
            } else {
                EV << "[RPL] unknown message from trickle " << msg->getKind() << endl;
                abort();
            }
        }

        // Free memory
        delete msg;

    } else {
        // Internal self-messages for timing

        // Root nodes only!
        if( msg->getKind() == init_timer_kind_self_message ){
            // Initialization timer triggered!

            // Time to wake trickle up...
            signalTrickle(rpl_init);

            // Initialize and set the reset timer
            reset_timer = new cMessage();
            reset_timer->setKind(reset_timer_kind_self_message);

            // Set the timer
            double resetAbsTime = simTime().dbl() + par("resetTime").doubleValue();
            scheduleAt(resetAbsTime, reset_timer);

            EV << "[RPL] Global reset scheduled at " << resetAbsTime  << endl;
        } else if( msg->getKind() == reset_timer_kind_self_message ){
            // Global reset timer triggered

            // Increase DODAG version
            dodagVersion++;

            // Time to reset trickle...
            signalTrickle(rpl_reset);

            // Re-set the timer
            double resetAbsTime = simTime().dbl() + par("resetTime").doubleValue();
            scheduleAt(resetAbsTime, reset_timer);
        }
    }

}

void RplEngine::signalTrickle(int kind){
    // Create a new message and send it out to the engine
    cMessage* msg = new cMessage("",kind);
    send(msg, "trickleOut");
}


void RplEngine::sendDioOut(){

    EV << "[RPL] DIO message with rank " << rank << endl;

    Enter_Method_Silent();

    // Create the DIO message
    DIOmessage *dioMessage = new DIOmessage();
    dioMessage->setType(ICMPv6_RPL);
    dioMessage->setCode(DIO);
    dioMessage->setRank(rank);
    dioMessage->setDODAGID(dodagID);
    dioMessage->setVersionNumber(dodagVersion);

    // Set IPV6 header fields
    IPv6Address dest=IPv6Address("FF02::1"); // IPv6 link-local multicast!

    IPv6ControlInfo *ctrlInfo = new IPv6ControlInfo();
    ctrlInfo->setDestAddr(dest);
    ctrlInfo->setSrcAddr(myIp);
    ctrlInfo->setProtocol(IP_PROT_IPv6_ICMP);

    dioMessage->setControlInfo(ctrlInfo);

    // Set pkt len TODO set it automatically
    dioMessage->setByteLength(DIO_LEN);

    // Send the packet out!
    send(PK(dioMessage),"ipv6Out");

}
