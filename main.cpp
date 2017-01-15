#include "MASS.h"
#include "Point.h"
#include "Cluster.h"
#include "Timer.h"	//Timer
#include <stdlib.h> // atoi
#include <unistd.h>
#include <vector>

Timer timer;

int main( int argc, char *args[] ) {
    //srand(1);
    // check that all arguments are present 
    // exit with an error if they are not
    if ( argc != 10 ) {
	cerr << "usage: ./main username password machinefile port nProc nThreads numTurns sizeX sizeY" << endl;
        return -1;
    }
  
    // get the arguments passed in
    char *arguments[4];
    arguments[0] = args[1]; // username
    arguments[1] = args[2]; // password
    arguments[2] = args[3]; // machinefile
    arguments[3] = args[4]; // port
    int nProc = atoi( args[5] ); // number of processes
    int nThr = atoi( args[6] );  // number of threads
    const int nIter = atoi( args[7] );	//Run this simulation for this many iterations
    const int sizeX = atoi( args[8] );
    const int sizeY = atoi( args[9] );
    const int myPopulation = 8; // arbitrary number of centroids for running purposes
    bool report = false;
    // initialize MASS with the machine information,
    // number of processes, and number of threads
    MASS::init( arguments, nProc, nThr );
    char* msg = (char*)("Hi\0");
    // Create the places.
    Places *centroids = new Places( 1, "Cluster", 1, msg, 3, 2, sizeX, sizeY );
    
    vector<int*> destinations;
    int north[2] = {0, 1};
    destinations.push_back( north );
    int south[2] = {0, -1};
    destinations.push_back( south );
    int east[2]  = {1, 0};
    destinations.push_back( east );
    int west[2]  = {-1, 0};
    destinations.push_back( west );

    // Create the agents.
    Agents *points = new Agents(2, "Point", msg, 3,centroids, myPopulation);
    centroids->callAll(Cluster::init_);
    timer.start();
    for(int i = 0; i < nIter; i++){
        // calculating mean
        points->callAll(Point::set_); // sets the weights
        if(report){
            MASS_base::log("pre move");
            points->callAll(Point::rept_);
        }
        points->callAll(Point::dist_);
        centroids->callAll(Cluster::update_);

        //moving points to new centroids
        
        centroids->callAll(Cluster::sig_);   // signals which neighbors exist
        centroids->exchangeBoundary();       // makes sure everyone has info on neighbors
        centroids->callAll(Cluster::findv_); // finds a spot to move unlocked info
        points->callAll(Point::move_);       // sets up the move
        points->manageAll();                 //migrate

        if(report){
            MASS_base::log("post move");
            points->callAll(Point::rept_);
        }
    }
    points->callAll(Point::rept_);
    long elapsedTime_END = timer.lap();
    cout << "Elapsed Time with " << nProc << " processors and " << nThr << " threads: " << elapsedTime_END << endl;
    MASS::finish( );
}
