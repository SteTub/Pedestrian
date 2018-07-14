#include"Agent_par.h"

//find starting point
coords findStart(grid city)
{
	coords output;

	for(int i=0;i<city.total_height;i++){
		for(int j=0;j<city.total_width;j++){
		
			if(city.location[i][j] == 3){	//searches until the start ( = 3 ) is found
				output.x = j;
				output.y = i;
				break;
			}
		}
	}
	return output;
}

//find end point
coords findEnd(grid city)
{
	coords output;

	for(int i=0;i<city.total_height;i++){
		for(int j=0;j<city.total_width;j++){
		
			if(city.location[i][j] == 4){	//searches until end point ( = 4) is found
				output.x = j;
				output.y = i;
				break;
			}
		}
	}
	return output;
}

//check if current grid box is clear
int pathBlocked(grid city, int x, int y)
{
	if(x < 0)
		return 1;
	if(y < 0)
		return 1;
	if(x > city.total_width)
		return 1;
	if(y > city.total_height)
		return 1;


	//if position is clear
	if(city.location[y][x] == 0)
		return 0;
	
	//if path is blocked
	else if(city.location[y][x] == 1)
		return 1;
	
	//if position is endpoint
	else if(city.location[y][x] == 4)
		return 0;
	
	//if position is startpoint
	else if(city.location[y][x] == 3)
		return 0;

	//if position is next to a wall
	return 2;
}

//this will flip the coordinate system so that (0,0) is bottom left, like a cartesian plane
int ConvertCartesianGrid(int x, int grid_height)
{
	return (grid_height - 1) - x;
}
