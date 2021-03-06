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

package inet.networklayer.rpl;

import inet.networklayer.rpl.trickle.ITrickle;
import inet.networklayer.rpl.trickle.TrickleNormal;
import inet.networklayer.rpl.RplEngine;

module RplRouting
{
    parameters:
        @display("i=block/control");

        string trickleType = default("TrickleNormal"); // type of the Trickle implementation

    gates:
        input ipv6In;
        output ipv6Out;

    submodules:
        trickle: <trickleType> like ITrickle {
            parameters:
                @display("p=252,43");
        }

        rplEngine: RplEngine {
            parameters:
                @display("p=141,43");
        }

    connections:
        ipv6In --> rplEngine.ipv6In;
        ipv6Out <-- rplEngine.ipv6Out;
        
        trickle.engineOut --> rplEngine.trickleIn;
        trickle.engineIn <-- rplEngine.trickleOut;
}
