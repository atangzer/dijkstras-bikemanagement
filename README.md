# Public Bike Management System

## Problem Description
*Adapted from Data Structures I.*

There exists a public bike service in which people can use to rent bicycles at their convenience and return the bicycles at any other designated station in the city. The public bike service has a Public Bike Management Center (PBMC) which is constantly observing the current state of all the stations in order to ensure that each station is in perfect condition. A perfect condition implies that a station has exactly half the number of bikes of its max capacity.

When a problem is reported at a specific station, PBMC will choose the shortest path go to that station. If there are multiple shortest paths to reach the specific problem station, then PBMC will choose the path that will require the least number of bikes to distribute. During the traversed path, if it encounters a station that is not in perfect condition, then it will either take or place bikes to the station in order to make it so.

To find the shortest path, we will first create a graph from the data that the user will input. They will input the maximum capacity of each station, total number of stations, the index of the problem station, how many bikes are in each current station, and the number of roads between each station, and the duration it will take to go from one station to another. Using Dijkstra’s Algorithm, we will use the graph we created from the input to find all possible shortest paths, then choose the one that requires the least number of bikes to be sent from the PBMC. Once the program has decided the optimal path, it will output the number of bikes that PBMC will need to send, the number of bikes to be collected at each station of the path, and the number of bikes PBMC will bring back to the centre.

## Inputs Specifications

```
Cmax, N, Sp, M 
Ci * N times (where Ci is the number of bikes at each station and N is the number from line 1 input)
(Station i Station j and Tij) * M Times (Tij is the time taken to travel from station i to j, and M is obtained from line 1 of input)
```

## Output Specifications

```
(# of bikes PBMC needs to send out) SPACE (# of bikes S1 needs to send) -> ... (# of bikes Sp needs to send) -> SPACE (# of bikes needed to be taken back to the PBMC)
```
