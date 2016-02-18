/*
 * ConnectedUniform.h
 *
 *  Created on: Nov 26, 2015
 *      Author: carlo
 */

#ifndef CONNECTEDUNIFORM_H_
#define CONNECTEDUNIFORM_H_

#include <StationaryMobility.h>

class ConnectedUniformMobility: public StationaryMobility {

protected:
    /** @brief Initializes the position according to the mobility model. */
    virtual void setInitialPosition();

public:
    ConnectedUniformMobility();
    virtual ~ConnectedUniformMobility();
};

#endif /* CONNECTEDUNIFORM_H_ */
