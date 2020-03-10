# route_planner_HU
Route planner application in C with Hungary's map and Dijkstra algorithm

This is my first big project in C as my homework for Programing 1 class.

The application uses a reduced format of Hungary's road network. It's able
to plan a route between two points with unlimited number of middle points.
The GUI used in the program is the SDL library.

The interface and how to use it:
On the left of the screen there is the map with the points the user can choose from.
When the vertex's color is red, it's active, otherwise it's not. The order of the 
chosen points on the route is the order the user have set.
There are two buttons on the right. With the first one you can reset the screen in
order to plan a new route. The bottom one can be used to plan the route.
After the planning, the data of the selected route are shown under the two buttons.
The name of the selected points with the distance till that point is shown.

Data structure:
The main structure is a graph with the vertexes as the locations and the edges as the 
distance between the two points. The graph can be found in a 2D array as its distance
matrix. There is another array with the vertexes and its coordinatates and also two arrays
containing the border points for the drawing function.

Source of data:
There are three .txt files for the project. The csp.txt contains the locations with an id, name 
and geological coordinates which will be stored in an array. The szak.txt is the source of the graph.
In each line of the file, there are the id of two points which will be connected and the distance between
these two locations. The third file is the hatar.txt which stores the border's data.
The data is from the Budapest University of Technology and Economics Department of Photogrammetry and
Geoinformatics.

Main algorithm:
The problem behind the application is called shortest path problem. There are a couple of great
solutions avaiable for this particular issue, however I've chosen the Dijkstra-algorithm mainly due to its speed.


