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

package inet.mobility.static;

import inet.mobility.common.MobilityBase;


//
//
simple ConnectedUniformMobility extends MobilityBase
{
    parameters:
        int rootX @unit(m) = default(0m);
        int rootY @unit(m) = default(0m);
        int rootZ @unit(m) = default(0m);
        double maxDistance @unit(m) = default(5m);
        double minDistance @unit(m) = default(5m);
        @class(ConnectedUniformMobility);
}

