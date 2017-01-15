#ifndef CLUSTER_H
#define CLUSTER_H
#include "Place.h"
#include "Point.h"
#include "MASS_base.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
using namespace std;

static const int placeholder = -999;
static const int range  = 20;

class Cluster : public Place {
public:
    static const int init_    = 0;
    static const int mean_    = 1;
    static const int update_  = 2;
    static const int sig_     = 3;
    static const int findv_   = 4;
    static const int getNext_ = 5;
    static const int rept_    = 6;

    Cluster(void* argument) : Place(argument) {
            bzero( arg, sizeof( arg ) );
            strcpy( arg, (char *)argument );
    };
    virtual void* callMethod(int functionId, void* arg) {
        switch(functionId) {
        case init_:
            return init();
        case mean_:
            return getMean(arg);
        case update_:
            return update();
        case sig_:
            return signal();
        case findv_:
            return findValid();
        case getNext_:
            return sendNextMove();
        case rept_:
            return report();
        }
        return NULL;
    };

private:
    char arg[100];
    int nextMove[2];
    static const int nextCluster[4][2];
    int status[4];
    int mean;
    int csize;
    vector<int*> movements;
    void* init();
    void* getMean(void* arg);
    void* signal();
    void* findValid();
    void* sendNextMove();
    void* update();
    void* report();
};
#endif