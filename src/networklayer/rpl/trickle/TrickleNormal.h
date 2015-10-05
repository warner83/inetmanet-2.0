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

// Class implementing the plain trickle

#ifndef TRICKLENORMAL_H_
#define TRICKLENORMAL_H_

#include <trickle/TrickleBase.h>

class TrickleNormal: public TrickleBase {

    // Initial interval
    double minInt;

    // Maximum number of doublings
    unsigned int maxDoublings;

    // Current interval value
    double curInt;

    // Redundancy value
    unsigned int redundancy;

    // Current messages received
    unsigned int numMessagesReceived;

    // Current number of doublings
    unsigned int curDoublings;

    virtual void intervalEnded();

    virtual void messageFired();

public:
    TrickleNormal(Rpl* r, double minI, unsigned int numDoubl, unsigned int k) : TrickleBase(r) {
        minInt = minI;
        maxDoublings = numDoubl;
        redundancy = k;
    }
    virtual ~TrickleNormal();

    virtual void initializeTrickle();

    virtual void reset();

    virtual void dioReceived();
};

#endif /* TRICKLENORMAL_H_ */
