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

package inet.networklayer.rpl.Statistics;

simple NodeEventCollector {
	parameters:
	    
        @signal[rank](type="long");
        @statistic[rankMetric](title="current rank"; source="rank"; record=vector,stats; interpolationmode=none);
        
      	@signal[shortestCost](type="long");
        @statistic[shortestCostMetric](title="shortest cost"; source="shortestCost"; record=vector,stats; interpolationmode=none);
        
      	@signal[minHops](type="long");
        @statistic[minHopsMetric](title="minimum number of hops to root"; source="minHops"; record=vector,stats; interpolationmode=none);
}