#include<iostream>

//array of environment
struct grid{
	int **location;		//condition of piece of grid, 0 = open space, 1 = wall/boundary, 2 = close to wall
	int total_height;	//dimensions of grid, in number of grid boxes
	int total_width;
	double box_height;	//dimensions of each grid point in metres
	double box_width;
};

//struct to move around x and y coordinates
struct coords{
	int x;
	int y;
};

coords findStart(grid city);				//find starting location (a 3 in the grid array)
coords findEnd(grid city);				//finds endpoint in grid (a 4 in the grid array)
int pathBlocked(grid city, int x, int y);		//returns success or failure depending on if path is clear or not
int ConvertCartesianGrid(int x, int grid_height);	//convert to a human readable cartesian coordinate system

