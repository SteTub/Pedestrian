#include"Agent_par.h"
#include"kd_tree.h"
#include<unistd.h>
#include<sys/time.h>
#include<fstream>
#include<cstdlib>
#include<cstddef>

#define MAX_AGENTS 50
#define MAX_TIMESTEP 50
#define REMOVE_STEP 5

/*
#define MAX_FOV 90.0
#define SIGHT_DIST 40.0
#define PERSON_SIZE 0.2		
#define PREFERRED_SPEED 1.4
#define COMFORT_WALL_DIST 0.7
#define PERSON_COMFORT_DIST 1.0 
*/

int main(int argc, char **argv){


//	srand(time(NULL));

	struct timeval start,end;
	std::ofstream time_file;

	//-----INITIALISE ENVIRONMENT-----//
	grid city;
/*
	//---UNCOMMENT TO LOAD IN ENVIRONMENT---//
	
//	std::ofstream fp;
	std::ifstream env_file;

	if(ROWS == 40)
		env_file.open("environment.txt", std::ifstream::in);
		//std::ifstream env_file("environment.txt", std::ifstream::in);
	else if(ROWS == 2000)
		env_file.open("environment2000_grid.txt", std::ifstream::in);
	else if(ROWS == 10000)
		env_file.open("environment10k.txt", std::ifstream::in);
	else if(ROWS == 20000)
		env_file.open("environment20k.txt", std::ifstream::in);
	else{
		std::cout << "NOT A DECLARED ENVIRONMENT\n";
		return 1;
	}

	if(!(env_file.is_open()))
	{
		std::cerr << "ERROR, FILE CLOSED\n";
		return 1;
	}

	city.total_height = ROWS;			//40 boxes tall and wide
	city.total_width = COLUMNS;
	city.box_height = BOX_SIZE;
	city.box_width = BOX_SIZE;

	//alloc size of grid array
	city.location = new int*[city.total_height];
	for(int i=0;i<city.total_height;++i)
		city.location[i] = new int[city.total_width];	

	//fill environment from file input
	for(int i=0;i<city.total_height;i++)
		for(int j=0;j<city.total_width;j++)
			env_file >> city.location[ConvertCartesianGrid(i, city.total_height)][j];	//uses cartesian coordinates, (0,0) is bottom left

	//close environment file
	env_file.close();

	coords start = findStart(city);
*/

	//----CREATES EMPTY ENVIRONMENT WITH BORDERS----//

	city.total_height = ROWS;
	city.total_width = COLUMNS;
	city.box_height = BOX_SIZE;
	city.box_width = BOX_SIZE;

	city.location = new int*[city.total_height];
	for(int i=0;i<city.total_height;i++)
		city.location[i] = new int[city.total_width];

	for(int i=0;i<city.total_height;i++)
	{
		for(int j=0;j<city.total_width;j++)
		{
			if(i == 0 || i == city.total_width - 1)
				city.location[i][j] = 1;
			else if(j == 0 || j == city.total_height - 1)
				city.location[i][j] = 1;
			else city.location[i][j] = 0;
		}
	}
	for(int i=1;i<city.total_height-1;i++){
		for(int j=1;j<city.total_width-1;j++)
		{							
			if(city.location[i-1][j] == 1 && city.location[i][j] == 0)
				city.location[i][j] = 2;
			if(city.location[i][j-1] == 1 && city.location[i][j] == 0)
				city.location[i][j] = 2;
			if(city.location[i+1][j] == 1 && city.location[i][j] == 0)
				city.location[i][j] = 2;
			if(city.location[i][j+1] == 1 && city.location[i][j] == 0)
				city.location[i][j] = 2;
		}
	}	

	city.location[city.total_height-4][city.total_height-4] = 4;

	//=====FINISH ENV INITIALIZATION=====//

	//-----INITIALISE AGENTS-----//
	
	std::vector<Person> my_agents;
	int x_start, y_start;
	position startpoint;
	coords endpoint;
	
	//find the endpoint in the city
	endpoint = findEnd(city);
	
	//declare end point
//	endpoint.x = ROWS-4;
//	endpoint.y = ROWS-4;	

	//size of area on each process
	int x_block_size = city.total_width;
	int y_block_size = city.total_height;

	//BOUNDARY VALUE OF EACH PROCESS
	double left_boundary = 0;		
	double right_boundary = left_boundary + x_block_size;			
	double down_boundary = 0;
	double up_boundary = down_boundary + y_block_size;

	//Initialise kd-tree
	node* root = NULL;

	//number of agents on each process
	int agent_number = MAX_AGENTS;

	gettimeofday(&start, NULL);
	

	//----INITIALIZE AGENTS----//
	for(int i=0;i<agent_number;i++)
	{
		x_start = (rand() % (int)x_block_size) + left_boundary;
		y_start = (rand() % (int)y_block_size) + down_boundary;

		int count=0;

		while(city.location[y_start][x_start] != 0)
		{
			x_start = (rand() % (int)x_block_size) + left_boundary;
			y_start = (rand() % (int)y_block_size) + down_boundary;
	
			count++;	
			if(count > x_block_size*y_block_size)
				break;
		}
		startpoint.x = (double)x_start;
		startpoint.y = (double)y_start;

		city.location[y_start][x_start] = 3;

		my_agents.push_back(Person(i, startpoint, endpoint, PERSON_SIZE, 0, PREFERRED_SPEED, 1.3*PREFERRED_SPEED, SIGHT_DIST, MAX_FOV, COMFORT_WALL_DIST, PERSON_COMFORT_DIST));

		//insert agent in tree
		root = insert(root, i, my_agents);
	}

	std::vector<agent_context> u_recv_agents;
	std::vector<agent_context> r_recv_agents;
	std::vector<agent_context> l_recv_agents;
	std::vector<agent_context> d_recv_agents;


	//-----BEGIN TIMESTEPS-----//
	for(double timestep=0;timestep<MAX_TIMESTEP;timestep++)
	{
	
		//-----MOVE AGENTS-----//
		for(size_t i=0;i<my_agents.size();i++)
		{	
			if(!my_agents[i].isRemoved())
				my_agents[i].makeMove(city, my_agents, root, u_recv_agents, r_recv_agents, d_recv_agents, l_recv_agents, up_boundary - PERSON_COMFORT_DIST, right_boundary - PERSON_COMFORT_DIST, down_boundary - PERSON_COMFORT_DIST, left_boundary - PERSON_COMFORT_DIST);
		}
		
	}


	gettimeofday(&end, NULL);
	
	double time_taken = (double)(end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)/1000000.0);

	std::cout << "===================\n";
	std::cout << "|    TIMINGS      |\n";
	std::cout << "===================\n";
	std::cout << "   SIZE = 1       \n";
	std::cout << "Time taken   = " << time_taken << std::endl;
	std::cout << "Agents       = " << agent_number << std::endl;
	std::cout << "Agents(total)= " << MAX_AGENTS << std::endl;
	std::cout << "Time steps   = " << MAX_TIMESTEP << std::endl;
	std::cout << "===================\n";
	

	for(int i=0;i<ROWS;i++)
		delete[] city.location[i];
	delete[] city.location;

	if(root != NULL)
		DeleteTree(root);		//remove existing elements
	delete root;			//delete root node


	return 0;
}
