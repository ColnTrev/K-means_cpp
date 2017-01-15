#include "Cluster.h"
#include "Point.h"
#include "MASS_base.h"
#include <cstdlib>
#include <cstring>
#include <sstream>
using namespace std;
extern "C" Agent* instantiate( void *argument ) {
  return new Point( argument );
}

extern "C" void destroy( Agent *object ) {
  delete object;
}

void* Point::setWeight(){
    weight = rand() % range + 1;
    lock   = false;
    return NULL;
}

void* Point::getWeight(void* arg){
    *((int*)arg) = weight;
    return NULL;
}

void* Point::dist(){
    int m;
    place->callMethod(Cluster::mean_, & m);
    int dist = abs(weight - m);
    if(dist <= m){
        lock = true;
    } else {
        lock = false;
    }
    return NULL;
}

void* Point::move(){
    if(!lock){
        //ensuring the weight stays with the agent
        int* next = (int*)(place->callMethod(Cluster::getNext_, NULL));
        vector<int> dest;
        dest.push_back(place->index[0] + next[0]);
        dest.push_back(place->index[1] + next[1]);
        migrate(dest); 
    }
    return NULL;
}
void* Point::report(){
    ostringstream convert;
    convert << "I am point: " <<  weight << "at centroid " << place->index[0]  << ' ' << place->index[1] << ". I am"<< (lock ? " locked" : " not locked");
    MASS_base::log(convert.str());
    return NULL;
}