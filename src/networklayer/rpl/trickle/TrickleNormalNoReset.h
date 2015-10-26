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

// Class implementing the plain trickle

#ifndef TrickleNormalNoReset_H_
#define TrickleNormalNoReset_H_

// Class implementing the regular Trickle

#include <trickle/TrickleNormal.h>

class INET_API TrickleNormalNoReset: public TrickleNormal {

public:
    TrickleNormalNoReset() ;

    virtual ~TrickleNormalNoReset();

    virtual void reset() {}; // Do nothing at reset

};

#endif /* TrickleNormalNoReset_H_ */
