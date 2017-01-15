#include "Cluster.h"
#include "Point.h"
#include "MASS_base.h"
#include <cstdlib>
#include <sstream>
using namespace std;

const int Cluster::nextCluster[4][2] = {{0,1},{1,0}, {0,-1},{-1,0}};

extern "C" Place* instantiate(void* arg) {
    return new Cluster(arg);
}

extern "C" void destroy(Place* obj) {
    delete obj;
}

void* Cluster::init(){
    int north[2]     = {0, 1};
    int east[2]      = {1, 0};
    int west[2]      = {-1, 0};
    int south[2]     = {0, -1};
    movements.push_back(north);
    movements.push_back(east);
    movements.push_back(west);
    movements.push_back(south);
    mean  = rand() % range + 1;
    csize = 0;
    return NULL;
}
void* Cluster::getMean(void* arg) {
    *((int*)arg) = mean;
    return NULL;
}
void* Cluster::signal(){
    outMessage_size = sizeof(int);
    outMessage = new int;
    *(int*)outMessage = (int)agents.size();
    return NULL;
}

void* Cluster::findValid(){
    int North[2] = {0, 1};
    int East[2]  = {1, 0};
    int South[2] = {0, -1};
    int West[2]  = {-1, 0};
	
	int *ptr = (int *)getOutMessage( 1, North );
    status[0] = ( ptr == NULL ) ? placeholder : *ptr;
    ptr = (int *)getOutMessage( 1, East );
    status[1] = ( ptr == NULL ) ? placeholder : *ptr;
    ptr = (int *)getOutMessage( 1, South );
    status[2] = ( ptr == NULL ) ? placeholder : *ptr;
    ptr = (int *)getOutMessage( 1, West );
    status[3] = ( ptr == NULL ) ? 0 : *ptr;
    
    vector<int> moveset;
	for(int i = 0; i < (int)movements.size(); i++) {
		if(status[i] != placeholder)
			moveset.push_back(i);
	}
    
	if(!moveset.empty()) {
		int r = rand() % moveset.size();
		nextMove[0] = nextCluster[moveset[r]][0];
		nextMove[1] = nextCluster[moveset[r]][1];
	}
    return NULL;
}
void* Cluster::sendNextMove(){
    int* retVal;
    retVal = new int[2];
    retVal[0] = nextMove[0];
    retVal[1] = nextMove[1];
    return retVal;
}

void* Cluster::update(){
    int sum     = 0;
    csize       = agents.size();
    if(csize > 0){
        for(int i = 0; i < csize; i++){
            int w;
            agents[i]->callMethod(Point::weight_, &w);
            sum+= w;
        }
        mean = sum / csize;
    }
    return NULL;
}

void* Cluster::report() {
    ostringstream convert;
    convert << "I am centroid: " << index[0] << " with mean: " << mean
         << "and have " << agents.size() << " points near me.";
    MASS_base::log(convert.str());
    return NULL;
}