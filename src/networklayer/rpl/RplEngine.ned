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

simple RplEngine{
      
    parameters:
        @display("i=block/control");
        
        // True only for root node
        bool isRoot = default(false);
        
        // Initial warm up time
        double initTime @unit("s") = default(1s);
        
        // Global reset timer
        double resetTime @unit("s") = default(100000000s);  
        
        // OF selected
        string objectiveFunctionType = default("OFzero");
        
        // Link estimator selected
        string linkEstimatorType = default("PerfectEtx");
                                       
    gates:
        input ipv6In;
        output ipv6Out;
        
        input trickleIn;
        output trickleOut;
    
}