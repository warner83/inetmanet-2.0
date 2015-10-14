/*
 * MRHOF.h
 *
 *  Created on: Oct 8, 2015
 *      Author: carlo
 */

#ifndef MRHOF_H_
#define MRHOF_H_

#include <OFBase.h>

#define PARENT_SWITCH_THRESHOLD 192

class MRHOF: public OFBase {
    double evaluateCost(struct parent* p){
        // Final cost as advRank + link_metric * 128
        p->cost = floor(p->rank + p->linkMetric * 128);
    }

    // Manage new parent with lower rank than mine
    void manageParentWithLowerRank(struct parent* p){
        // First remove parent exceeding the threshold
        removeExceedingParent(p->cost);

        if(parentSetFull()){
           // Parent set is still full remove worse parent
           removeWorse();
        }

        // Eventually add the parent
        addParent(p);
    }

    // Manage change of rank among parents
    void manageChangeRank() {
        // Remove parents exceeding the threshold
        double min = getBest();
        removeExceedingParent(parent_set[min].cost);

        // Set the new preferred parent if needed
        if(preferred_parent == NULL)
            preferred_parent = &parent_set[min];

    }


    int getWorse(){
        double max = 0;
        int pos = -1;
        for(int i = 0; i < parent_size; ++i ){
            if(max < parent_set[i].cost)
            {
                max = parent_set[i].cost;
                pos = i;
            }
        }
        return pos;
    }

    int getBest(){
        double min = INFINITE_RANK;
        int pos = -1;
        for(int i = 0; i < parent_size; ++i ){
            if(min > parent_set[i].cost)
            {
                min = parent_set[i].cost;
                pos = i;
            }
        }
        return pos;
    }

    void removePos(int i){
        // Compact vector
        for(int j = i; j < parent_size-1; ++j ){
            parent_set[j] = parent_set[j+1];
        }

        parent_size--;
    }

    void removeExceedingParent(double cost){

        // Remove all the nodes with a rank greater than cost + PARENT_SWITCH_THRESHOLD
        for(int i = parent_size-1; i >= 0; --i ){
            if(parent_set[i].cost > ( cost + PARENT_SWITCH_THRESHOLD ) ){

                if(preferred_parent != NULL  && preferred_parent->addr == parent_set[i].addr){
                    // Preferred parent will be removed
                    preferred_parent = NULL;
                }

                removePos(i);
            } else {
               EV << "not removing" << endl;
            }
        }
    }

    void removeWorse(){
        // Parent set is still full, remove the worst
        int max = getWorse();
        if(max != -1) // the parent set can be empty
            removePos(max);
    }

    void addParent(struct parent* p) {
        // Add the parent
        parent_set[parent_size] = *p;
        parent_size++;

        EV << "added" << endl;

        // Preferred parent has been erased -> evaluate the new preferred parent
        if(preferred_parent == NULL){
            int pos = getBest();
            preferred_parent = &parent_set[pos];
        }
    }

    // Evaluate and return the rank of the node according to the parent set
    double evaluateRank(){
        if(preferred_parent == NULL || parent_size == 0)
            return INFINITE_RANK;

        // First term, path cost of the preferred parent
        double first = preferred_parent->cost;

        // Second term, MinHopRankIncrease * (1 + floor(Rank/MinHopRankIncrease)) where rank is the rank in the parent set with the highest rank
        // Third term The largest calculated Rank among paths through the parent set, minus MaxRankIncrease.
        double second = 0;
        double third = 0;
        // Find the node with the highest rank
        for(int i = 0; i < parent_size; ++i ){
            if(parent_set[i].cost > second )
                second = parent_set[i].cost;

            if(parent_set[i].cost > third)
                third = parent_set[i].cost;
        }
        second = MIN_HOP_RANK_INCREASE * (1 + floor(second/MIN_HOP_RANK_INCREASE));

        third = third - MAX_RANK_INCREASE;
        if(third < 0)
            third = 0;

        // The rank of the node is the highest between the three
        double ret = 0;
        if(first > second)
            ret = first;
        else
            ret = second;

        if(third > ret)
            ret = third;

        return ret;
    }


public:
    MRHOF(bool isRoot, LinkEstimatorBase* l, EventCollector* e, int pp_size);
    virtual ~MRHOF();
};

#endif /* MRHOF_H_ */
