#include<mpi.h>
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

	int size, rank;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
		
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sqrt_size = sqrt(size);
	int up_agent_size;
	int down_agent_size;
	int left_agent_size;
	int right_agent_size;

	srand(time(NULL));

	struct timeval start,end,loopstart,loopend;
	std::ofstream time_file;

	
	//write out time for each loop to file
	//currently only works for predefined file names
	//potential idea to overcome this is to use bash script
/*
	if(rank == 0){
		if(size == 4)
			time_file.open("strong500_2000_4.txt");
		time_file << "# Timings for " << size << " nodes\n";
	}
*/
	//-----DEFINE DERIVED MPI STRUCT-----//

//	int count = 3;
	int blocklengths[3] = {3,2,14};
	MPI_Datatype oldtypes[3] = {MPI_INT, MPI::BOOL, MPI_DOUBLE};
	
	MPI_Aint disps[3];
	MPI_Datatype old_agent_context;
	MPI_Datatype mpi_agent_context;

	disps[0] = 0;
	disps[1] = offsetof(agent_context,current_dest);
	disps[2] = offsetof(agent_context,x_pos);

	MPI_Type_create_struct(3, blocklengths, disps, oldtypes, &old_agent_context);
	MPI_Type_create_resized(old_agent_context,0,(MPI_Aint)sizeof(struct agent_context), &mpi_agent_context);
	MPI_Type_commit(&mpi_agent_context);

	//=====FINISH STRUCT CREATION	
	//----INITIALISE CARTESIAN TOPOLOGY----//
	
	MPI_Comm cart_comm;
	int ndims = 2;
	int reorder = 1;
	int dims[2], pbc[2];
	dims[0] = sqrt_size;
	dims[1] = sqrt_size;
	pbc[0] = 0;
	pbc[1] = 0;

	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims,pbc,reorder,&cart_comm);	
	int source;
	int upper_neighbour;
	int lower_neighbour;
	int right_neighbour;
	int left_neighbour;
	MPI_Cart_shift(cart_comm, 0, 1, &source, &upper_neighbour);
	MPI_Cart_shift(cart_comm, 0, -1, &source, &lower_neighbour);
	MPI_Cart_shift(cart_comm, 1, 1, &source, &right_neighbour);
	MPI_Cart_shift(cart_comm, 1, -1, &source, &left_neighbour);

	//=====FINISH CART INITIALIZATION=====//

	//-----INITIALISE ENVIRONMENT-----//
	//currently all processes do this 
	grid city;
/*
	//----LOAD ENVIRONMENT----//

	//DECLARE FILE I/O
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

	//coords start = findStart(city);
*/

	//----OPEN ENVIRONMENT----//

	//STRONG SCALING
	city.total_height = ROWS;
	city.total_width = COLUMNS;

	//WEAK SCALING
//	city.total_height = ROWS*sqrt_size;
//	city.total_width = COLUMNS*sqrt_size;

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
	
	endpoint.x = city.total_width-4;
	endpoint.y = city.total_height-4;	

	//size of area on each process
	int x_block_size = city.total_width/sqrt_size;
	int y_block_size = city.total_height/sqrt_size;

	//BOUNDARY VALUE OF EACH PROCESS
	double left_boundary = x_block_size*(rank % sqrt_size);			
	double right_boundary = left_boundary + x_block_size;			
	double down_boundary = y_block_size*((int)(rank/sqrt_size));
	double up_boundary = down_boundary + y_block_size;

	//Initialise kd-tree
	node* root = NULL;

	//number of agents on each process
	int agent_number = MAX_AGENTS/size;

	//start timer on rank 0
	if(rank == 0){
		gettimeofday(&start, NULL);
	}

	//create startpoint of each agent
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

		my_agents.push_back(Person(i+(rank*agent_number), startpoint, endpoint, PERSON_SIZE, 0, PREFERRED_SPEED, 1.3*PREFERRED_SPEED, SIGHT_DIST, MAX_FOV, COMFORT_WALL_DIST, PERSON_COMFORT_DIST));

		//insert agent in tree
		root = insert(root, i, my_agents);
	}
	//=====FINISH AGENT INITIALIZATION=====//

	if(rank == 0)
		gettimeofday(&loopstart, NULL);

	//-----BEGIN TIMESTEPS-----//
	for(double timestep=0;timestep<MAX_TIMESTEP;timestep++)
	{

		//halo exchanged sent agents
		std::vector<agent_context> left_agents;
		std::vector<agent_context> right_agents;
		std::vector<agent_context> upper_agents;
		std::vector<agent_context> lower_agents;
		//agents to be received
		std::vector<agent_context> l_recv_agents;
		std::vector<agent_context> r_recv_agents;
		std::vector<agent_context> u_recv_agents;
		std::vector<agent_context> d_recv_agents;
		//agents which have moved from process to process
		std::vector<agent_context> transfer_u_agents;
		std::vector<agent_context> transfer_r_agents;
		std::vector<agent_context> transfer_d_agents;
		std::vector<agent_context> transfer_l_agents;
		//agents received on move
		std::vector<agent_context> transf_recv_u_agents;
		std::vector<agent_context> transf_recv_r_agents;
		std::vector<agent_context> transf_recv_d_agents;
		std::vector<agent_context> transf_recv_l_agents;
	
		std::vector<int> removed_indexes;

		//-----GATHER AGENTS-----//
		
		gatherBorderAgents(my_agents, root, upper_agents, 0, up_boundary - PERSON_COMFORT_DIST, 0);
		gatherBorderAgents(my_agents, root, right_agents, 1, right_boundary - PERSON_COMFORT_DIST, 0);
		gatherBorderAgents(my_agents, root, lower_agents, 2, down_boundary + PERSON_COMFORT_DIST, 0);
		gatherBorderAgents(my_agents, root, left_agents, 3, left_boundary + PERSON_COMFORT_DIST, 0);

		//=====FINISH GATHERING AGENTS=====//
		
		//-----SEND AND RECV BOUNDARY AGENTS-----//

		//---SEND UP / RECEIVE FROM BELOW
		//send agents context UP
		MPI_Send(&upper_agents[0], upper_agents.size(), mpi_agent_context, upper_neighbour, 0, cart_comm);

		//find out size of message from lower neighbour
		MPI_Probe(lower_neighbour, 0, cart_comm, &stat);	
		MPI_Get_count(&stat, mpi_agent_context, &down_agent_size);
	
		d_recv_agents.resize(down_agent_size);
	
		//receive from below
		MPI_Recv(&d_recv_agents[0], d_recv_agents.size(), mpi_agent_context, lower_neighbour, 0, cart_comm, &stat);

		//---SEND DOWN / RECEIVE FROM ABOVE
		//send agents context DOWN
		MPI_Send(&lower_agents[0], lower_agents.size(), mpi_agent_context, lower_neighbour, 0, cart_comm);

		//find out size of message from UPper neighbour
		MPI_Probe(upper_neighbour, 0, cart_comm, &stat);	
		MPI_Get_count(&stat, mpi_agent_context, &up_agent_size);
		u_recv_agents.resize(up_agent_size);
	
		MPI_Recv(&u_recv_agents[0], u_recv_agents.size(), mpi_agent_context, upper_neighbour, 0, cart_comm, &stat);
		
		//---SEND RIGHT / RECEIVE FROM LEFT
		//send agents context RIGHT
		MPI_Send(&right_agents[0], right_agents.size(), mpi_agent_context, right_neighbour, 0, cart_comm);

		//find out size of message from LEFT neighbour
		MPI_Probe(left_neighbour, 0, cart_comm, &stat);	

		MPI_Get_count(&stat, mpi_agent_context, &left_agent_size);
		l_recv_agents.resize(left_agent_size);
	
		MPI_Recv(&l_recv_agents[0], l_recv_agents.size(), mpi_agent_context, left_neighbour, 0, cart_comm, &stat);
		
		//---SEND LEFT / RECEIVE FROM RIGHT
		//send agents context RIGHT
		MPI_Send(&left_agents[0], left_agents.size(), mpi_agent_context, left_neighbour, 0, cart_comm);
	
		//find out size of message from RIGHT neighbour
		MPI_Probe(right_neighbour, 0, cart_comm, &stat);	

		MPI_Get_count(&stat, mpi_agent_context, &right_agent_size);
		r_recv_agents.resize(right_agent_size);
	
		MPI_Recv(&r_recv_agents[0], r_recv_agents.size(), mpi_agent_context, right_neighbour, 0, cart_comm, &stat);
	
		//=====FINISH SEND AND RECVS=====//
		
		//-----MOVE AGENTS-----//
		for(size_t i=0;i<my_agents.size();i++)
		{	
			if(!my_agents[i].isRemoved())
				my_agents[i].makeMove(city, my_agents, root, u_recv_agents, r_recv_agents, d_recv_agents, l_recv_agents, up_boundary - PERSON_COMFORT_DIST, right_boundary - PERSON_COMFORT_DIST, down_boundary - PERSON_COMFORT_DIST, left_boundary - PERSON_COMFORT_DIST);
		}
		//=====FINISH MOVING AGENTS=====//


		//-----TRANSFER AGENTS IF REQUIRED-----//	
		
		gatherBorderAgents(my_agents, root, transfer_u_agents, 0, up_boundary, 0);
		gatherBorderAgents(my_agents, root, transfer_r_agents, 1, right_boundary, 0);
		//PROBLEM HERE, SEG FAULT IF NOT COMMENTED
		//gatherBorderAgents(my_agents, root, transfer_d_agents, 2, down_boundary, 0);
		//gatherBorderAgents(my_agents, root, transfer_l_agents, 3, left_boundary, 0);

		//-----SEND AND RECV BOUNDARY AGENTS-----//

		//---SEND UP / RECEIVE FROM BELOW
		//send agents context UP
		MPI_Send(&transfer_u_agents[0], transfer_u_agents.size(), mpi_agent_context, upper_neighbour, 0, cart_comm);

		//find out size of message from lower neighbour
		MPI_Probe(lower_neighbour, 0, cart_comm, &stat);	
		MPI_Get_count(&stat, mpi_agent_context, &down_agent_size);
		d_recv_agents.resize(down_agent_size);
		//receive from below
		MPI_Recv(&d_recv_agents[0], d_recv_agents.size(), mpi_agent_context, lower_neighbour, 0, cart_comm, &stat);
		
		//---SEND DOWN / RECEIVE FROM ABOVE
		//send agents context DOWN
		MPI_Send(&transfer_d_agents[0], transfer_d_agents.size(), mpi_agent_context, lower_neighbour, 0, cart_comm);

		//find out size of message from UPper neighbour
		MPI_Probe(upper_neighbour, 0, cart_comm, &stat);	
		MPI_Get_count(&stat, mpi_agent_context, &up_agent_size);
		u_recv_agents.resize(up_agent_size);
	
		MPI_Recv(&u_recv_agents[0], u_recv_agents.size(), mpi_agent_context, upper_neighbour, 0, cart_comm, &stat);
	
		//---SEND RIGHT / RECEIVE FROM LEFT
		//send agents context RIGHT
		MPI_Send(&transfer_r_agents[0], transfer_r_agents.size(), mpi_agent_context, right_neighbour, 0, cart_comm);

		//find out size of message from LEFT neighbour
		MPI_Probe(left_neighbour, 0, cart_comm, &stat);	

		MPI_Get_count(&stat, mpi_agent_context, &left_agent_size);
		l_recv_agents.resize(left_agent_size);
	
		MPI_Recv(&l_recv_agents[0], l_recv_agents.size(), mpi_agent_context, left_neighbour, 0, cart_comm, &stat);
		
		//---SEND LEFT / RECEIVE FROM RIGHT
		//send agents context RIGHT
		MPI_Send(&transfer_l_agents[0], transfer_l_agents.size(), mpi_agent_context, left_neighbour, 0, cart_comm);
	
		//find out size of message from RIGHT neighbour
		MPI_Probe(right_neighbour, 0, cart_comm, &stat);	

		MPI_Get_count(&stat, mpi_agent_context, &right_agent_size);
		r_recv_agents.resize(right_agent_size);
	
		MPI_Recv(&r_recv_agents[0], r_recv_agents.size(), mpi_agent_context, right_neighbour, 0, cart_comm, &stat);
	
		//=====FINISH TRANSFERRING AGENTS=====//
	
		//-----ADD NEW AGENTS-----//	
		for(size_t i=0;i<u_recv_agents.size();i++)
		{	
			//add new agent to vector
			if(my_agents.size() > 0){
				my_agents.push_back(ConstructPerson(u_recv_agents[i]));

				//add new index to kd tree
				root = insert(root, my_agents.size()-1, my_agents);		
			}
		}
		for(size_t i=0;i<r_recv_agents.size();i++)
		{	
			//add new agent to vector
			if(my_agents.size() > 0){
				my_agents.push_back(ConstructPerson(r_recv_agents[i]));

				//add new index to kd tree
				root = insert(root, my_agents.size()-1, my_agents);		
			}
		}
		for(size_t i=0;i<d_recv_agents.size();i++)
		{	
			//add new agent to vector
			if(my_agents.size() > 0){
				my_agents.push_back(ConstructPerson(d_recv_agents[i]));

			//add new index to kd tree
				root = insert(root, my_agents.size()-1, my_agents);		
			}
		}
		for(size_t i=0;i<l_recv_agents.size();i++)
		{
			//add new agent to vector
			if(my_agents.size() > 0){
				my_agents.push_back(ConstructPerson(l_recv_agents[i]));

				//add new index to kd tree
				root = insert(root, my_agents.size()-1, my_agents);		
			}
		}

		//=====FINISHED ADDING NEW AGENTS=====//
		//-----"REMOVE" AGENTS-----//	

		for(size_t i=0;i<transfer_u_agents.size();i++)
		{
			int agent_index = indexFromID(root, transfer_u_agents[i].id, transfer_u_agents[i].x_pos, transfer_u_agents[i].y_pos, my_agents);
			//set the agent value as being removed
			if(my_agents.size() > 0)
				my_agents[agent_index].setRemovedTrue();
			//delete agent from tree			
			if(root != NULL)	
				root = deleteNode(root, transfer_u_agents[i].id, agent_index, my_agents);			
		}

		for(size_t i=0;i<transfer_r_agents.size();i++)
		{
			int agent_index = indexFromID(root, transfer_r_agents[i].id, transfer_r_agents[i].x_pos, transfer_r_agents[i].y_pos, my_agents);
		
			if(my_agents.size() > 0)
				my_agents[agent_index].setRemovedTrue();			
			if(root != NULL){
				root = deleteNode(root, transfer_r_agents[i].id, agent_index, my_agents);			
			}		
		}

		for(size_t i=0;i<transfer_d_agents.size();i++)
		{			
			int agent_index = indexFromID(root, transfer_d_agents[i].id, transfer_d_agents[i].x_pos, transfer_d_agents[i].y_pos, my_agents);
			if(my_agents.size() > 0)
				my_agents[agent_index].setRemovedTrue();			
			if(root != NULL)
				root = deleteNode(root, transfer_d_agents[i].id, agent_index, my_agents);			
		}

		for(size_t i=0;i<transfer_l_agents.size();i++)	
		{
			int agent_index = indexFromID(root, transfer_l_agents[i].id, transfer_l_agents[i].x_pos, transfer_l_agents[i].y_pos, my_agents);
			if(my_agents.size() > 0)
				my_agents[agent_index].setRemovedTrue();			
			if(root != NULL)
				root = deleteNode(root, transfer_l_agents[i].id, agent_index, my_agents);			
		}

	//=====FINISHED "REMOVING" AGENTS=====//

	//----BEGIN REBALANCING OF TREE AND ACTUAL REMOVAL OF OLD OPERATORS----//
		//after a number of time steps
		if((int)timestep % REMOVE_STEP == REMOVE_STEP - 1){
	
			for(int i=0;i<(int)my_agents.size();i++)
			{
				//if agent is in list of agents to remove, then erase from the vector	
				if(my_agents[i].isRemoved())
				{
					my_agents.erase(my_agents.begin() + i);
				}
			}

			//Reconstruct tree
			if(root != NULL)
				DeleteTree(root);		//remove existing elements
			root = NULL;			//reset root node
			
			//insert updated vector to tree
			for(int i=0;i<(int)my_agents.size();i++){
				root = insert(root, i, my_agents);
			}

		}

		//----PRINT TIMINGS TO FILE IF FILE EXISTS----//
		if(rank == 0){
			gettimeofday(&loopend, NULL);
			double loop_time = (double)(loopend.tv_sec - loopstart.tv_sec) + ((loopend.tv_usec - loopstart.tv_usec)/1000000.0);
			if(time_file.is_open())
				if((int)timestep % 10)
					time_file << loop_time << " " << timestep << std::endl;
		}

	}

	//print results to rank 0
	if(rank == 0){
		gettimeofday(&end, NULL);
	
		double time_taken = (double)(end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)/1000000.0);

		std::cout << "===================\n";
		std::cout << "|    TIMINGS      |\n";
		std::cout << "===================\n";
		std::cout << "Time taken   = " << time_taken << std::endl;
		std::cout << "Agents       = " << agent_number << std::endl;
		std::cout << "Agents(total)= " << MAX_AGENTS << std::endl;
		std::cout << "No. of Ranks = " << size << std::endl;
		std::cout << "Time steps   = " << MAX_TIMESTEP << std::endl;
		std::cout << "===================\n";
	}

//	if(rank == 0)
//		time_file.close();

	for(int i=0;i<ROWS;i++)
		delete[] city.location[i];
	delete[] city.location;

	if(root != NULL)
		DeleteTree(root);		//remove existing elements
	delete root;			//delete root node

	MPI_Type_free(&mpi_agent_context);
	MPI_Type_free(&old_agent_context);

	MPI_Comm_free(&cart_comm);

	MPI_Finalize();
	return 0;
}
