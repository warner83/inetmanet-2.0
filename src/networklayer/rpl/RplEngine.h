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

#ifndef RPL_H_
#define RPL_H_

// Class implementing the RPL routing engine

#include <omnetpp.h>

#include "INETDefs.h"

#include "IInterfaceTable.h"
#include "IPv6InterfaceData.h"
#include "InterfaceTableAccess.h"
#include "IPv6ControlInfo.h"

#include "OF/OFBase.h"

#define WPAN_INTERFACE 0

#define DIO_LEN         28 // TODO check this!!

class INET_API RplEngine : public cSimpleModule {

    ///////////  System pointers  ///////////
    IInterfaceTable *ift; // Pointer to the interface table
    InterfaceEntry* ie; // Pointer to the WPAN interface entry

    ///////////  Internal structures  ///////////

    // My IPv6 address
    IPv6Address myIp;

    // Am I root?
    bool isRoot;

    // Initialized? set to true when the first DIO is received. Always true if root!
    bool initialized;

    // Current rank of the node
    unsigned int rank;

    // Current preferred parent
    IPv6Address preferredParent;

    // Current DODAGID
    IPv6Address dodagID;

    // Current DODAG version
    unsigned int dodagVersion;

    // Objective function
    OFBase* of;

    ///////////  Timers  ///////////

    // Initialization timer, for warm up
    cMessage* init_timer;

    // Reset timer, for global reset
    cMessage* reset_timer;

    ///////////  RPL functions  ///////////

    // Omnet functions
    void initialize(int stage);

    virtual int numInitStages()const { return 4; }

    void handleMessage(cMessage *msg);

    // Initialize engine
    void engineInitialize();

    ///////////  Trickle event signal functions  ///////////

    void signalTrickle(int kind);

public:
    RplEngine();
    virtual ~RplEngine();

    // Send out a DIO message, used by trickle implementations
    void sendDioOut();

};

#endif /* RPL_H_ */
