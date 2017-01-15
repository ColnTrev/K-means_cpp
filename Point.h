#ifndef POINT_H
#define POINT_H
#include "Agent.h"
#include "Cluster.h"
#include "MASS_base.h"
#include <cstring>
#include <sstream>
const int threshold = 10;
using namespace std;
class Point : public Agent {
    public:
        static const int set_    = 0;
        static const int move_   = 1;
        static const int weight_ = 2;
        static const int dist_   = 3;
        static const int rept_   = 4;
        Point( void *argument ) : Agent( argument ) {
                bzero(arg, sizeof(arg));
                strcpy(arg, (char*)argument);
        };
        virtual void* callMethod(int functionId, void* argument){
            switch(functionId){
                case set_   : return setWeight();
                case move_  : return move();
                case weight_: return getWeight(argument);
                case dist_: return dist();
                case rept_  : return report();
            }
            return NULL;
        };
    private:
        char arg[100];
        int cid;
        int weight;
        bool lock;
        void* setWeight();
        void* getWeight(void* argument);
        void* dist();
        void* move(); // moves the centroid to the next group of places
        void* report();
};
#endif