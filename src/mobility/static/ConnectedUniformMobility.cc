/*
 * ConnectedUniform.cc
 *
 *  Created on: Nov 26, 2015
 *      Author: carlo
 */

#include <ConnectedUniformMobility.h>

#include "ObstacleControl.h"
#include "ChannelControl.h"

Define_Module(ConnectedUniformMobility);

ConnectedUniformMobility::ConnectedUniformMobility() {
    // TODO Auto-generated constructor stub

}

ConnectedUniformMobility::~ConnectedUniformMobility() {
    // TODO Auto-generated destructor stub
}


void ConnectedUniformMobility::setInitialPosition()
{
    static std::vector<Coord> installedNodes(0);
    static std::vector<int> neighPerNodes(0);

    //numHosts = par("numHosts");

    ObstacleControl* obstacles = ObstacleControlAccess().getIfExists();

    //ChannelControl* cc = dynamic_cast<ChannelControl*>(simulation.getModuleByPath("channelControl"));

    double maxDistance = /*cc->getInterferenceRange(NULL)*/ par("maxDistance").doubleValue();
    double minDistance = par("minDistance").doubleValue();

    if(installedNodes.size() == 0){
        lastPosition.x = par("rootX");
        lastPosition.y = par("rootY");
        lastPosition.z = par("rootZ");
    } else {

        do{

            int numNodes = installedNodes.size();

            // Get the node with the lowest number of nodes
            int min = neighPerNodes[0];
            int minIndex = 0;
            for(int i = 1; i < numNodes; i++ ){
                if(neighPerNodes[i] <= min){
                    min = neighPerNodes[i];
                    minIndex = i;
                }
            }

            // Get the neigh node
            int neighNode = minIndex ;
            neighPerNodes[neighNode]++;

            // Get angle
            double angle = uniform(0, 360);

            double rad = PI * angle / 180;
            Coord direction(cos(rad), sin(rad));

            // Get distance
            double distance = uniform(minDistance, maxDistance); // Halving the max distance just to have good links...

            direction *= distance;

            lastPosition = installedNodes[neighNode];
            lastPosition += direction;

            // Lets force the playground
            lastPosition.x = std::min(lastPosition.x, par("constraintAreaMaxX").doubleValue());
            lastPosition.y = std::min(lastPosition.y, par("constraintAreaMaxY").doubleValue());

            lastPosition.x = std::max(lastPosition.x, par("constraintAreaMinX").doubleValue());
            lastPosition.y = std::max(lastPosition.y, par("constraintAreaMinY").doubleValue());

            lastPosition.z = 0;

        } while (obstacles->inObstacle(lastPosition));
    }

    // Check and update neigh info
    int myNeigh = 0;
    for(int i = 0; i< installedNodes.size() ; i++){
        if( lastPosition.distance(installedNodes[i]) <= maxDistance ){
            neighPerNodes[i]++;
            myNeigh++;
        }
    }

    installedNodes.push_back(lastPosition);
    neighPerNodes.push_back(myNeigh);

    recordScalar("x", lastPosition.x);
    recordScalar("y", lastPosition.y);
    recordScalar("z", lastPosition.z);
}
