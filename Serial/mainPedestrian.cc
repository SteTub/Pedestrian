#include "Agent.h"
#include<fstream>
#include<time.h>
#include<unistd.h>

#define MAX_FOV 90.0
#define SIGHT_DIST 30.0
#define PERSON_SIZE 0.2		
#define PREFERRED_SPEED 1.4
#define COMFORT_WALL_DIST 0.7
#define PERSON_COMFORT_DIST 3.0 

#define MAX_AGENTS 1		//number of agents

int main(int argc, char **argv)
{	
	bool graphics=false;
	int agent_no = 1;	
	int c, index;
	
	opterr = 0;
	while((c = getopt(argc,argv,"n:g")) != -1){
		switch(c)
		{
			case 'n':
				agent_no = atoi(optarg);
				if(agent_no < 0){
					std::cout << "Number of agents must be positive\n";
					return 1;
				}
				break;
			case 'g':
				graphics=true;
				break;
			case '?':
				if(optopt == 'n')
					std::cout << "Error: Option -" << optopt << " requires an argument\n";
				else if(isprint(optopt))
					std::cout << "Error: flag -" <<optopt << " is unknown\n";
				else	std::cout << "Unknown option\n";
				return 1;
			default:
				abort();
		}
	}
	for(index = optind; index <argc; index++)
		std::cout << "Non-option argument: '" << argv[index] << "'\n";
	//set precision of output for checking purposes
	std::cout.precision(6);

	srand(time(NULL));	

	//----DEFINE ENVIRONMENT----//
	grid city;

	//DECLARE FILE I/O
	std::ofstream fp;
	std::ifstream env_file("environment.txt", std::ifstream::in);
	
	if(!(env_file.is_open()))
	{
		std::cerr << "ERROR, FILE CLOSED\n";
		exit(1);
	}

/*
	//----CREATE NEW ENVIROMENT----//
	//set up environment
	city.total_height = ROWS;			//40 boxes tall and wide
	city.total_width = COLUMNS;
	city.box_height = BOX_SIZE;
	city.box_width = BOX_SIZE;
	
	//alloc size of grid array
	city.location = new int*[city.total_height];
	for(int i=0;i<city.total_height;++i)
		city.location[i] = new int[city.total_width];	

	//put a border around map
	for(int i=0;i<city.total_height;i++){
		for(int j=0;j<city.total_width;j++){
			if(i == 0 || i == city.total_width-1)
				city.location[i][j] = 1;
			else if(j == 0 || j == city.total_height-1)
				city.location[i][j] = 1;

			else city.location[i][j] = 0;

		}
	}

*/
	
	//PREDEFINED ENVIRONMENT
	city.total_height = ROWS;		//change ROWS depending on file size		
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
	
	//set these value for graphics program
	for(int i=0;i<city.total_height;i++)
		for(int j=0;j<city.total_width;j++)
			xy_grid.push_back(city.location[i][j]);

	//---SET UP AGENTS---//

	std::vector<Person> agents;
	int x_start, y_start;
	coords startpoint;
		
	//initialise start and end points
	coords ending = findEnd(city);
	for(int i=0;i<MAX_AGENTS;i++){

		x_start = rand() % city.total_width;
		y_start = rand() % city.total_height;

		//check spawn point isn't inside an obstacle
		while(pathBlocked(city, x_start, y_start) != 0){
			x_start = rand() % city.total_width;
			y_start = rand() % city.total_height;
		}

		startpoint.x = x_start;
		startpoint.y = y_start;
	//	city.location[y_start][x_start] = 3;
	
		//add agent to array
		agents.push_back(Person(i, startpoint, ending, PERSON_SIZE, PREFERRED_SPEED, SIGHT_DIST, MAX_FOV, COMFORT_WALL_DIST, PERSON_COMFORT_DIST));
	}

	//----RUN THE AGENTS----//
	for(int time_step = 0;time_step < 1000;time_step++){
		for(int i=0;i<agents.size();i++){
	
			agents[i].makeMove(city, agents, x_radar, y_radar);
		}
	}

	//PRINT DETAILS OF EACH AGENT

	for(int i=0;i<MAX_AGENTS;i++){

		std::cout << "AGENT ID = [" << agents[i].myID() << "]\n";
		std::cout << "-------------------------\n";
	//	std::cout << "Starting point = (" << agents[i].xHist(0) << ", " << agents[i].yHist(0) << ")\n";
	//	std::cout << "Ending point = (" << ending.x << ", " << ending.y << ")\n";
		if(agents[i].reachedDestination() == true)
			std::cout << "I reached my goal!!!\n";
		else std::cout << "I got lost\n";
		std::cout << "=========================\n";

	}
	
	//---THIS IS ONLY FOR A SINGLE PERSON, MOSTLY TO CHECK THE ALGORITHM OR GRAPHICS---//

	//USE THIS TO SEE THE GRAPHIC OF A SINGLE RAY CAST 
//	agents[0].rayCastGraphic(city, angle, x_radar, y_radar);		
	
	//cn be used to visualise the radar scan
//	agents[0].radarVision(city, x_radar, y_radar);

/*
	//open file to print out the agents history
	fp.open("path.txt");	
	
	if(!(fp.is_open()))
	{
		std::cerr << "ERROR, PATH FILE IS NOT OPEN\n";
		exit(1);
	}
*/
/*
	//---PRINT HISTORY TO FILE AND SCREEN---//
	for(int i=0;i<agents[0].HistSize();i++){
		fp << agents[0].xHist(i) << " " << agents[0].yHist(i) << std::endl;			//write history to file
*/

	//need to store all agents telemetry in a new vector for graphical purposes.
//	int agent_max = ((agents.size()<10) ? agents.size() : 10);
//	for(int j=0;j<agent_max;++j){ 

	//currently only works for up to 5 agents
	//this is because each vector needs to be defined in advance for the graphics
	if(MAX_AGENTS ==1){
		for(int i=0;i<agents[0].HistSize();++i){
			x_path.push_back(agents[0].xHist(i));	
			y_path.push_back(agents[0].yHist(i));	
		}
	}
/*
	else if(MAX_AGENTS ==2){
		for(int i=0;i<agents[1].HistSize();++i){
			x_path.push_back(agents[1].xHist(i));	
			y_path.push_back(agents[1].yHist(i));	
		}
	}
	else if(MAX_AGENTS ==3){
		for(int i=0;i<agents[2].HistSize();++i){
			x_path.push_back(agents[2].xHist(i));	
			y_path.push_back(agents[2].yHist(i));	
		}
	}
	else if(MAX_AGENTS==4){
		for(int i=0;i<agents[3].HistSize();++i){
			x_path.push_back(agents[3].xHist(i));	
			y_path.push_back(agents[3].yHist(i));	
		}
	}
	else if(MAX_AGENTS==5){
		for(int i=0;i<agents[4].HistSize();++i){
			x_path.push_back(agents[4].xHist(i));	
			y_path.push_back(agents[4].yHist(i));	
		}
	}
*/
	//-----DISPLAY GRAPHICS-----//
	if(graphics == true){
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
		glutInitWindowSize(600,600);
		glutCreateWindow("Radar Vision");
		glutDisplayFunc(display_callback);
	//	glutReshapeFunc(reshape_callback);
		init();
		glutMainLoop();
	}
	//free array			
	for(int i=0;i<40;i++)
		delete[] city.location[i];
	delete[] city.location;

	return 0;
}
