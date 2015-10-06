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

#include "trickle/ITrickle.h"
#include "trickle/TrickleNormal.h"

Define_Module(RplEngine);

RplEngine::RplEngine() {
}

RplEngine::~RplEngine() {
}

void RplEngine::initialize(int stage)
{
    if(stage == 3){

        EV<<"RPL: Recover system pointers\n";

        // Recover system pointers and my address
        ift = InterfaceTableAccess().get(); // Interface table
        ie = ift->getInterface(WPAN_INTERFACE); // WPAN interface entry
        myIp = ie->ipv6Data()->getLinkLocalAddress();

        EV <<"RPL: My IPv6 is " << myIp.str() << endl;

        // TODO check this mess...
        cModule* parent = getParentModule();
        if( parent->findSubmodule("trickle") >= 0 ){
            cModule* mod = parent->getSubmodule("trickle");
            trickleTimer = (ITrickle *) mod; // Trickle instance
            trickleTimer->setRplPtr(this); // Set myself for callback
        }
        else {
            EV << "No trickle found in " << getParentModule()->getFullPath() << endl;
            abort();
        }
    }
}

void RplEngine::handleMessage(cMessage *msg)
{

    if (!msg->isSelfMessage())
    {
        EV<<"RPL: RPL message received\n";

        RPLmessage *rplMessage = check_and_cast<RPLmessage *>(msg);

        if(rplMessage->getCode() == DIO){
            DIOmessage *dioMessage = check_and_cast<DIOmessage *>(msg);
            EV<<"RPL: DIO message received from " << rplMessage->getSrc().str() << " DODAGID " << dioMessage->getDODAGID().str() << " rank " << dioMessage->getRank() << endl;

            // TODO check consistency

            // Signal trickle that a message has been received
            trickleTimer->dioReceived();
        }

    }
    else
    {
        // Self messages?
    }
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
