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
#include "ICMPv6Message_m.h"
#include "IPv6Datagram.h"

#include "trickle/TrickleEvents.h"

#define init_timer_kind_self_message        1

Define_Module(RplEngine);

RplEngine::RplEngine() {
    // Nothing to do here
}

RplEngine::~RplEngine() {
    delete init_timer;
}

void RplEngine::initialize(int stage)
{
    if(stage == 1){
        isRoot = par("isRoot").boolValue();

        if(isRoot){
            initialized = true;

            // If root set timer for initialization
            simtime_t initAbsTime = simTime() + par("initTime").doubleValue();

            // Initialize the timer
            init_timer = new cMessage();
            init_timer->setKind(init_timer_kind_self_message);
            scheduleAt(initAbsTime, init_timer);

            EV << "RPL: Trickle will start at " << initAbsTime << endl;
        } else {
            // RPL will be inactive until the first DIO is received
            initialized = false;
        }

    } else if(stage == 3){

        EV<<"RPL: Recover system pointers\n";

        // Recover system pointers and my address
        ift = InterfaceTableAccess().get(); // Interface table
        ie = ift->getInterface(WPAN_INTERFACE); // WPAN interface entry
        myIp = ie->ipv6Data()->getLinkLocalAddress();

        EV <<"RPL: My IPv6 is " << myIp.str() << endl;

    }
}

void RplEngine::handleMessage(cMessage *msg)
{

    if (!msg->isSelfMessage())
    {
        if( strcmp(msg->getArrivalGate()->getFullName(), "ipv6In" ) == 0 ){
            // Message form the network

            EV<<"RPL: RPL message received\n";

            RPLmessage *rplMessage = check_and_cast<RPLmessage *>(msg);

            if(rplMessage->getCode() == DIO){
                DIOmessage *dioMessage = check_and_cast<DIOmessage *>(msg);
                EV<<"RPL: DIO message received from " << rplMessage->getSrc().str() << " DODAGID " << dioMessage->getDODAGID().str() << " rank " << dioMessage->getRank() << endl;

                // TODO check consistency

                if( !initialized ){
                    // This is my first DIO message
                    initialized = true;

                    // Let's wake trickle up!
                    signalTrickle(rpl_init);

                }

                // Signal trickle that a message has been received
                signalTrickle(consistant_message_received);

            } else {
                EV << "RPL: unknown rpl message from network " << rplMessage->getCode() << endl;
                abort();
            }
        } else if( strcmp(msg->getArrivalGate()->getFullName(), "trickleIn" ) == 0 ){
            // Internal message from trickle
            if( msg->getKind() == send_dio_message ){
                // Fire a DIO message
                sendDioOut();
            } else {
                EV << "RPL: unknown message from trickle " << msg->getKind() << endl;
                abort();
            }
        }


        // Free memory
        delete msg;

    } else {
        // Internal self-messages for timing

        if( msg->getKind() == init_timer_kind_self_message ){
            // Initialization timer triggered!

            // Time to wake trickle up...
            signalTrickle(rpl_init);

            // Anything else to be initialized??
        }
    }

}

void RplEngine::signalTrickle(int kind){
    // Create a new message and send it out to the engine
    cMessage* msg = new cMessage("",kind);
    send(msg, "trickleOut");
}


void RplEngine::sendDioOut(){

    EV << "RPL: DIO message with rank " << 0 << endl; // TODO change rank

    Enter_Method_Silent();

    // Create the DIO message
    DIOmessage *dioMessage = new DIOmessage();
    dioMessage->setType(ICMPv6_RPL);
    dioMessage->setCode(DIO);
    dioMessage->setRank(0); // TODO set rank
    //dioMessage->setDODAGID(myAddress); // TODO set DODAGID

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
