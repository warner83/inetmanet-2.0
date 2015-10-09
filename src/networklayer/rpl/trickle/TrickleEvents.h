/*
 * TrickleEvents.h
 *
 *  Created on: Oct 7, 2015
 *      Author: carlo
 */

#ifndef TRICKLEEVENTS_H_
#define TRICKLEEVENTS_H_

// Trickle--Engine events exchange

// trickle -> engine
#define send_dio_message                1

// engine -> trickle
#define rpl_reset                       2
#define message_received                3
#define rpl_init                        4

#endif /* TRICKLEEVENTS_H_ */
