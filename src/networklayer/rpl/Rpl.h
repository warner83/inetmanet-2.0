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

#include <omnetpp.h>

#include "INETDefs.h"

#include "IInterfaceTable.h"
#include "IPv6InterfaceData.h"
#include "InterfaceTableAccess.h"
#include "IPv6ControlInfo.h"

#define I_timer_kind_self_message         1

#define WPAN_INTERFACE 0

#define DIO_LEN         28 // TODO check this!!

class Rpl : public cSimpleModule {

    ///////////  System pointers  ///////////
    IInterfaceTable *ift; // Pointer to the interface table
    InterfaceEntry* ie; // Pointer to the WPAN interface entry

    ///////////  RPL functions  ///////////
    void initialize(int stage);

    virtual int numInitStages()const { return 4;}

    void handleMessage(cMessage *msg);

    void sendDioOut();

    ///////////  Trickle functions  ///////////

    cMessage *I_timer;

    void trickleInitialize();

    void intervalEnded();

public:
    Rpl();
    virtual ~Rpl();
};

#endif /* RPL_H_ */
