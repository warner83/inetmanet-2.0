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

cplusplus {{
#include "IPv6Address.h"
#include "ICMPv6Message_m.h"
}}

class noncobject IPv6Address;

packet ICMPv6Message;

//
// RPL message definition
//

// RPL messages types
enum RPL_messages_types
{
    DIS = 0;
    DIO = 1;
    DAO = 2;
    DAO_ACK= 3;
}

packet RPLmessage extends ICMPv6Message {
    int code enum(RPL_messages_types);  
    
    // Mgmt information collected at the IPv6 layer and passed to RPL
	IPv6Address src;
}
