# Route planner with GUI
This project is a route planner application which is capable of finding the shortest path between two locations of Hungary.

## General info
The data used here is a simplified version of Hungary's road network from the Budapest University of Technology and Economics.
It is stored in a 2D array as its adjecency matrix which is later used for planning the shortest route with Dijkstra's algorithm.

## Interface
For the GUI the SDL library is used. Use this to plan any route between two loactions with unlimited number of middle points:
 * The first click selects the starting point of the route
 * Add the chosen cities to the route by clicking on them (if it is red, it is on the route)
 * Click on planning (Tervezés)
 * Examine the optimal route both on the map and in the list
 
If you want to plan a new route:
 * Click on new route (Új útvonal)
 * Follow the steps discribed above
