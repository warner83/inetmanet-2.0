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

package inet.networklayer.rpl.trickle;

import inet.networklayer.rpl.trickle.ITrickle;

//
// TODO auto-generated type
//
simple TrickleF like ITrickle
{
       parameters:
            @display("i=block/cogwheel_s");
            double minInt @unit("s") = default(8ms);
			int redundancy = default(8);
			int maxDoublings = default(20);
					
		gates:
        	input engineIn;
        	output engineOut;
}
