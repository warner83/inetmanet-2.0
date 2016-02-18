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

#ifndef TRICKLES_H_
#define TRICKLES_H_

// Class implementing Trickle-S

#include <TrickleNormal.h>

class TrickleS: public TrickleNormal {

    // Number of suppressed DIO messages
    int suppressedDIOs;

    void initialize(int stage);

    void intervalEnded();

    // This function is added to handle TrickleS dynamics, that allows gaps between intervals
    void intervalBegin();

    void messageFired();

    void initializeTrickle();

public:
    TrickleS();
    virtual ~TrickleS();


};

#endif /* TRICKLES_H_ */
