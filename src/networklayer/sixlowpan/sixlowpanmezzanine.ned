//
// Copyright (C) 2012 Jonas Hartwig, BTU Cottbus
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

package inet.networklayer.sixlowpan;

import inet.networklayer.sixlowpan.sixlowpan;
import inet.linklayer.IOutputQueue;

//
// Implements the wrapper for Contiki's 6LoWPAN protocol implementation.
//
// The 6LoWPAN packet is represented by the _6lowpanDatagram message class.
//
module sixlowpanmezzanine 
{

	parameters:
	    string ifqType; // name of the interface queue type

    gates:
        input fromUpper;
        output toUpper;
        input fromDown;
        output toDown;
        
   	submodules:
        
    six: sixlowpan {
            parameters:
                 @display("p=70,180;i=block/layer");
        }
        
    ifq: <ifqType> like IOutputQueue {
            parameters:
                @display("p=70,80;i=block/passiveq;q=IFQueue");
        }
        
    connections:
        
    six.fromIPv6 <-- ifq.out;
    ifq.in <-- fromUpper;
        
    toUpper <-- six.toIPv6;
    fromDown --> six.fromMac;
    toDown <-- six.toMac;
}
