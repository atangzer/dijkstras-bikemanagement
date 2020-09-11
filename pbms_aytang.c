#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 501
#define INFINITY 1000000

typedef int Vertex; 

/*To represent a node (each bike station), a struct is used.
A structure tag GNode[i][j] is used to keep track of the route into 
of the path from station i to station j.

	Nv: Identifies vertex Vi
	Ne: Counts # of paths that goes to Vi
	Send: Total # of bikes to be sent to Vi
	Back: Total # of bikes to be set back after Vi is reached*/

struct GNode {
    int Nv;
    int Ne;
    int Send;
    int Back;
} GNode[NMAX][NMAX];

/*Global variables for input and for use in Dijkstra's Algorithm:

	Cmax: Max capacity of bikes per station (Cmax <= 100); 
		  can also be interpreted as the total # of nodes
	N: Total # of stations (N <= 500)
	Sp: States the indexing of the bike stations
	M: Total # of roads; represents # of edges
	bikeStatus[]: The amount of bikes available at each station
	G[][]: Stores the travel time between adjacent stations
	dist[]: Keeps track of the shortest distance between vertex V and 
			adjacent vertices (W)
	path[]: Keeps track of where the shortest path comes from
	known[]: Marks whether or not a vertex has been visited*/

int Cmax, N, Sp, M;
int bikeStatus[NMAX], G[NMAX][NMAX];
int dist[NMAX], path[NMAX], known[NMAX];

//buildG() builds the graph according to user input
void buildG();

/*shortestDist() uses Dijkstra's Algorithm to find all possible 
shortest paths */

void shortestDist();

//bestPath() determines the best path and prints it out
void bestPath();

int main() {
	int i, j;

	scanf("%d %d %d %d", &Cmax, &N, &Sp, &M); //user inputs parameters

	for (i = 1; i <= N; i++) {
		scanf("%d", &bikeStatus[i]);	//input how many bikes are at each station
		for (j = 1; j <= N; j++) {		//initialize the parameters of each node
			G[i][j] = 0;		
			GNode[i][j].Ne = GNode[i][j].Nv = 0;
			GNode[i][j].Back = GNode[i][j].Send = 0;
		}
	}

	buildG();

	shortestDist();

	bestPath();

	return 0;
}

void buildG() {
	for (int i = 0; i < M; i++) { //M is the amount of roads specified by input
		int si, sj, tij;	//user types in the adjacent stations and travel time
		scanf("%d %d %d", &si, &sj, &tij);
		G[si][sj] = tij;	//information is stored in G[][]; ensures that each
		G[sj][si] = tij;	//station are mutually adjacent to one and another
	}
}

void shortestDist() {
	int i, j;
	int min, cvw, temp;

	//initialize graph variables; they will be overwriten later
	for (i = 1; i <= N; i++) {
		dist[i] = INFINITY; //tracks shortest distance 
		path[i] = INFINITY; //tracks number of shortest paths to V
		known[i] = 0;
	} 

	//initialize PBMC attributes; represented by indices 0 in GNode
	dist[0] = 0;
	GNode[0][0].Nv = GNode[0][0].Ne = -1;
	GNode[0][0].Send = GNode[0][0].Back = 0;
	path[0] = 1;

	//The main part of Dijkstra's Algorithm; where all the magic happens...
	Vertex V, W;

	for (;;) {

		if (known[Sp]) {	//if the final station has been visited;
			break;			//break out of the loop
		}

		/*find an unvisited vertex with the smallest unknown distance
		and update found vertex as the current vertex being looked at*/
		min = INFINITY;
        V = -1;

        for (i = 0; i <= N; i++) {
            if (known[i] == 0 && dist[i] < min) {
                min = dist[i];
                V = i;
            } 
        }

        if (V == -1) { //not a vertex
        	break;
        }

        known[V] = 1;	//Mark vertex/station as visited

        for (W = 1; W <= N; W++) {
        	/*CASE 1: There exists 2 adjacent stations that can't be reached
        	or we have already visited, so it will be skipped*/
        	if (known[W] || G[V][W] == 0) {
        		continue;
        	}
        	//cvw holds the the travel time from station A to C via B
        	cvw = dist[V] + G[V][W];
      		/*CASE 2: The supposed travel time between V and W takes longer
      		than the current travel time so it will be skipped*/
        	if (dist[W] < cvw) {
        		continue;
        	//CASE 3: A path with a shorter travel time has been found.
        	} else if (dist[W] >= cvw || dist[W] == INFINITY) {
        		if (dist[W] > cvw){
        			path[W] = 0;	//Reset the current station with the shortest path
									//all the shortest paths found before are disregarded
        		}
        		dist[W] = cvw;		//update length of path

        		/*Evaluate the # of bikes that need to be sent away or delivered to current station
        		temp determines the bike difference and how far off the station is from
        		being at perfect condition*/
	        	if ((Cmax / 2) < bikeStatus[W]) { //cmax / 2 = perfect condition
					//over perfect condition
	        		temp = bikeStatus[W] - (Cmax / 2);
	        	} else {
	        		temp = (Cmax / 2) - bikeStatus[W];
	        	}

        		i = 0; //holds number of paths to vertex V
        		j = path[W]; //set j to the last vertex of the path of adjacent vertex W

				/*Looks at the current path from W to the station at vertex V;
				Determines # of bikes needed and # of bikes needed to be sent out
				to neighboring stations*/

	        	while (i < path[V]) { //i < # of shortest paths from V; has not yet reached station at vertex V
	        		GNode[W][j].Nv = V;	//sets the current vertex V to Nv of GNode 
	        		GNode[W][j].Ne = i; //set current # of paths leading to current vertex V to Ne

	        		/*If the # of bikes at the current station is at less than half-capacity,
        			then bikes are sent over*/

	        		if (bikeStatus[W] < (Cmax / 2)) {
	        			//Check how many bikes need to be sent to current station Vi
	        			if (GNode[V][i].Back >= temp) { //If the difference is less than # of bikes to be sent out
						/*GNode[Vertex][j] represents the path at Vertex where j tracks 
		  				each visited vertex of the path*/
	        				GNode[W][j].Send = GNode[V][i].Send;	//# of bikes to be sent OUT doesn't change
	        				GNode[W][j].Back = GNode[V][i].Back - temp;	//# of bikes to be sent BACK decreases
	        			} else { //increase # of bikes to send out 
	        				GNode[W][j].Send = temp + GNode[V][i].Send - GNode[V][i].Back;
	        				GNode[W][j].Back = 0;
	        			}
	        		} else { ////current # of bikes at vertex W >= perfect condition
	        			/*If the # of bikes at the current station is at greater than half-capacity,
        				then bikes are sent back to the PBMC*/
	        			GNode[W][j].Send = GNode[V][i].Send;	//# of bikes to be sent OUT doesn't change
	        			GNode[W][j].Back = temp + GNode[V][i].Back;	//# of bikes to be sent BACK increases
	        		}
	        		i++; //increment current path counter
	        		j++; //check next vertex of path
	        	}
        		path[W] = j;	//update the # of shortest paths to the current adjacent vertex W
        	}
        }
	}
}

void bestPath() {
	int i, t1, t2;
	int sPBMC = INFINITY, bPBMC = INFINITY; //tracks # of bikes to be sent out and back to PBMC
	int bikePath[NMAX]; //tracks # of bikes to be sent out at each station; the best path

	//Compare the possible shortest paths produced by shortestDist().
	i = 0; //counts the number of paths to vertex Sp

	//determines # of bikes send back and out of PBMC
	while (i < path[Sp]) {
		/*Look for the path with the least amount of bikes to be sent out by the PBMC, then
		look for the one with the amount of bikes to be sent back*/
		if ((sPBMC == GNode[Sp][i].Send && bPBMC > GNode[Sp][i].Back) || sPBMC > GNode[Sp][i].Send) {
			//update values for PBMC
			sPBMC = GNode[Sp][i].Send;	//# of bikes to send out from path #i 
			bPBMC = GNode[Sp][i].Back; 	//# of bikes to send back from path #i;
			//t1 and t2 are placeholders to store the last station's information
			t1 = GNode[Sp][i].Nv; //current vertex of path #i
			t2 = GNode[Sp][i].Ne; //path that goes to the vertex at t1
		}
		i++; //look at the next path
	}

	//Set up parameters for tracing back the path obtained from while loop above
	int j = t2; //path at Sp
	int ind = 0;
	i = t1; //vertex of station Sp

	/*Begin trace and append the # of bikes into bikePath[].
	Note that the station order is reversed.
	Update the parameters as the array is appended*/
	while (i >= 0) { //i >= index of first station
		bikePath[ind++] = i;
		t1 = GNode[i][j].Nv; //go to vertex before Sp
		t2 = GNode[i][j].Ne;
		i = t1;
		j = t2;
	}

	//PBMC send/back values are printed seperated
	printf("%d ", sPBMC);

	//Print the path from the end of the array.
	--ind;
	while (ind >= 0) {
		printf("%d->", bikePath[ind]);
		ind--;
	}

	printf("%d %d", Sp, bPBMC);
}