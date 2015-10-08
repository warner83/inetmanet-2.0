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

#include <OFBase.h>

OFBase::OFBase(bool root) {
    parent_size = 0;
    isRoot = root;
    if(!isRoot)
        myrank = INFINITE_RANK;
    else
        myrank = MIN_HOP_RANK_INCREASE;

    historic_min_rank = INFINITE_RANK;
}

OFBase::~OFBase() {
    // TODO Auto-generated destructor stub
}

unsigned int OFBase::addrToIndex(IPv6Address a){
    return a.words()[3];
}

void OFBase::printParentSet(){

    EV << "[OF] parent set size " << parent_size << endl;
    for(int i = 0; i < parent_size; ++i){
        EV << "[OF] parent [" << i << "] " << parent_set[i].index << " cost " << parent_set[i].cost << endl;
    }

    if(preferred_parent != NULL){
        EV << "[OF] preferred parent " << preferred_parent->index << endl;
    } else {
        EV << "[OF] no preferred parent" << endl;
    }
}

bool OFBase::process_dio(IPv6Address srcAddr, DIOmessage *dioMessage) {
    if(isRoot)
        // Lazy root, ignore other DIOs
        return false;

    // Create temporary descriptor
    struct parent np(dioMessage->getRank(), 1, 0, srcAddr, addrToIndex(srcAddr) ); // TODO get link metric!

    // Evaluate final cost
    evaluateCost(&np);

    EV << "[OF] processing dio from " << np.index << " rank " << np.rank << " cost " << np.cost << " link " << np.linkMetric <<endl;

    if(preferred_parent != NULL)
        EV << "[OF] preferred parent " << preferred_parent->index << " rank " << preferred_parent->cost << endl;

    bool ret = false;

    // Check if the node belongs to the P set
    int pos = getParent(np.addr);
    if( pos == -1 ){
        // Node is new check its rank
        if( myrank >= np.cost && // Is the new candidate better than the one I've now?
            rankAcceptable(np.cost) // Is the rank acceptable?
            ) {

            // The new node should be part of the parent set

            EV << "[OF] new node acceptable rank with lower cost" << endl;

            manageParentWithLowerRank(&np);

        }
    } else {
        // The node belongs to the parent set, check if the rank is the same
        if( np.cost != parent_set[pos].cost ){
            // TODO in case the rank is greater check if the rank is acceptable, if not trigger local repair

            EV << "[OF] update rank of a parent" << endl;

            // I update the rank and the parent set
            parent_set[pos].cost = np.cost;
            parent_set[pos].rank = np.rank;
            parent_set[pos].linkMetric = np.linkMetric;

            // Manage the change
            manageChangeRank();
        }
    }

    // Evaluate new rank
    double newRank = evaluateRank();

    printParentSet();

    // Check if the rank changed
    if(newRank != myrank){

        EV << "[OF] got new rank " << newRank << " was " << myrank << " preferred " << preferred_parent->index << endl;

        myrank = newRank;
        ret = true;
    }

    // Update historic min, if needed
    if(myrank < historic_min_rank)
        historic_min_rank = myrank;

    return ret;

}

void OFBase::reset(){
    EV << "[OF] reset" << endl;

    parent_size = 0;
    if(!isRoot)
        myrank = INFINITE_RANK;
    else
        myrank = MIN_HOP_RANK_INCREASE;

    historic_min_rank = INFINITE_RANK;
}



