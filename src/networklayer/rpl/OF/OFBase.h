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

#ifndef OFBASE_H_
#define OFBASE_H_

#include <cownedobject.h>

#include <omnetpp.h>
#include "IPv6InterfaceData.h"
#include "IPv6ControlInfo.h"

#include "DIOmessage_m.h"

#include "RplDefs.h"

// This class defines the interface for all the objective functions

// Parent set management structure
struct parent{
    double rank; // Origin rank
    double linkMetric; // Link metric
    double cost; // overall cost (OF dependent)
    IPv6Address addr;
    int index;

    parent(double r, double lm, double c, IPv6Address a, int i ){
        rank = r;
        linkMetric = lm;
        cost = c;
        index = i;
        addr = a;
    }

    parent(){
        rank = 0;
        linkMetric = 0;
        cost = 0;
    }
};


class OFBase: public cOwnedObject {

protected:

    std::vector<struct parent> parent_set; // Parent set
    struct parent* preferred_parent; // Preferred parent
    double myrank; // My current Rank
    int parent_size; // Current parent size
    int max_parent_size; // Max parent size

    double historic_min_rank;
    bool isRoot;

    // Functions for managing the parent set

    // Utility function to convert the IPv6 address to an index. The less significant 32 bits are taken to create the index
    unsigned int addrToIndex(IPv6Address a);

    // Return the internal index of parents, return -1 if no parents are found
    int getParent(IPv6Address addr){
        for(int i = 0; i < parent_size; ++i)
            if( addr == parent_set[i].addr )
                return i;

        return -1;
    }

    // Return true if parent set is full
    bool parentSetFull(){
        return parent_size == max_parent_size;
    }

    // Return true if the rank is acceptable: over MIN_HOP_RANK_INCREASE and less than historic_min + MAX_RANK_INCREASE
    bool rankAcceptable(double rank){
        return rank > MIN_HOP_RANK_INCREASE && rank < (historic_min_rank + MAX_RANK_INCREASE);
    }

    // Evaluate overall node cost, based origin rank and link metric
    virtual double evaluateCost(struct parent* p) = 0;

    // Evaluate node rank based on the parent set
    virtual double evaluateRank() = 0;

    // Manage new parent with lower rank
    virtual void manageParentWithLowerRank(struct parent* p) = 0;

    // Manage change in the rank of a parent
    virtual void manageChangeRank() = 0;

    // Debug purposes: print parent set
    void printParentSet();


public:

    OFBase( bool root );

    virtual ~OFBase();

    // RPL interface functions

    // Return current parent size
    int getParentSize(){
        return parent_size;
    }

    // Return address of the preferred parent
    IPv6Address getPP(){
        return preferred_parent->addr;
    }

    // Return the rank to update RPL structures
    double getRank(){
        return myrank;
    }

    // Function that processes inbound DIO messages. The function return:
    // -  True if the dio was considered consistent, false otherwise. Latter -> trickle reset
    // -  False if the dio was considered consistent, true otherwise. Latter -> trickle reset
    bool process_dio(IPv6Address srcAddr, DIOmessage *dioMessage);

    // Reset parent set
    void reset();
};

#endif /* OFBASE_H_ */
