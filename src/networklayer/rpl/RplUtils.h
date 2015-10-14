/*
 * RplUtils.h
 *
 *  Created on: Oct 8, 2015
 *      Author: carlo
 */

#ifndef RPLUTILS_H_
#define RPLUTILS_H_

// Utility function to convert the IPv6 address to an index. The less significant 32 bits are taken to create the index
static unsigned int addrToIndex(IPv6Address a){
    unsigned int n = a.words()[3] & 0xFF;
    return n - 1 ;
}

#define NODE_INDEX addrToIndex(myIp) - 1

#endif /* RPLUTILS_H_ */
