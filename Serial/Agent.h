#include<iostream>
//#include<cstring>	//not currently needed (i think)
#include<vector>
#include<cmath>
#include"Environment.h"

//#ifdef GRAPHICS
	#include "visual_grid.h"
//#endif

#define NORTH 0
#define EAST 90
#define SOUTH 180
#define WEST 270

#define JUMP_CRITERIA 2		//requirement for jump criteria

#define TIME_STEP 0.5 		//0.5 of a second 

#define BOX_SIZE 0.5
#define BOX_SQUARE BOX_SIZE*BOX_SIZE

#define PI 3.14159265358979323846


//struct to return details about a ray after a raycast
struct paths{
	int open_path;
	double probable_path;
};
//struct to return details of a wall or location
struct location_info{
	double angle;
	double distance;
};
//return vector positions
struct position{
	double x;
	double y;
};

//---AGENT CLASS---//
class Person {

public:
	//Default constructor
	Person();
	//Constructor 
	Person(int id, coords start, coords destination, double in_person_size, double in_preferred_speed, double in_sight_distance, double in_max_fov, double in_comfort_wall_distance, double in_person_comfort_distance);
	//Copy Constructor
	Person(const Person& person_in);

	//Destructor
	~Person();

	//movement functions
	void makeMove(grid city, std::vector<Person> nearby_people, std::vector<int> &x_radar, std::vector<int> &y_radar);
	void findTargetAngle(grid city);	//find desired path, return the angle of the target location
	void updateVelocity(grid city, std::vector<Person> nearby_people);		//updates velocity, includes wall forces
	void moveForward();
	void checkValidMove(grid city);		//NOT USED
	void rotate(double theta);		//NOT USED
	int isAtDestination();			//check if at destination
	bool reachedDestination();

	//vision functions
	double checkOpenPath(int open_path, double input_angle, double current_choice, double diff, int wall_check); //return the direction of the best open path
	double checkProbablePath(double current_path, double previous_path, double input_angle, double init_angle, double current_choice, grid city);	//return direction of best probable path
	double navigateCorner(grid city, double current_angle);		//modifies the probable path angle to avoid getting close to corner
	double radarVision(grid city, std::vector<int> &x_radar, std::vector<int> &y_radar);	//scans through a range of angles using raycasts
	paths rayCast(grid city, double ray_angle, double view_distance);	//send a ray out through the grid in a straight line from current position
	void rayCastGraphic(grid city, double ray_angle, std::vector<int> &x_radar, std::vector<int> &y_radar);	//ray cast for graphic
	
	//NOT USED
	//void drawLine(grid city, double ray_angle);

	//collision functions
	position avoidPeople(std::vector<Person> nearby_people);			
	location_info findNearestWall(grid city, int on_right);		//finds the nearest wall and returns the distance and angle to it

	//functions to access private members
	double myID() const;		//access ID of agent
	double xPos() const;		//access x position
	double yPos() const;		//access y position
	double mySpeed() const;
	double myOrientation() const;

//	int curStep() const;		//access current number of moves
	double xHist(int i) const;	//access history at point 'i'
	double yHist(int i) const;
	int HistSize();			//size of history
	double mySize() const;		//person size
	
	//functions to update private members		
	void setInitialPosition(coords start);		//set starting coordinates
	void setDestination(coords ending);		//set destination coords
	void setSightDistance(double distance);		//set maximum sight distance
	void setOrientation(double orientation);	//modify orientation
	void setInitialOrientation();			//set initial orientation
	void updateGoalAngle();				//reset angle to destination using current orientation
	
private:
	//Position components
	double x_pos, y_pos;	//current position
	double x_dest, y_dest;	//position of destination
	//orientations
	double orientation;	//this is the angle the person is facing, in degrees
	double target_orientation;	//desired orientation
	double goal_angle;		//angle to endpoint from current position

	//for corners
	bool current_dest;		//if true, agent currently has a target destination
	coords current_goalpoint;	//coordinates of current goal
	double current_goal_distance;	//distance to current objective
	bool turn_corner_left;		//if true, corner is to the left so person should rotate to left to see around corner

	//keep history for graphical purposes 
	std::vector<double> x_history;		//FOR GRAPHICS
	std::vector<double> y_history;		//FOR GRAPHICS

	//Personal attributes
	int id;				//specific identifier
	double person_size;		//size of person, this will be radius of disk
	double current_speed;		//movement distance = speed x timestep
	double preferred_speed;		//preferred personal speed, tries to maintain this
	double max_speed;		//maximum possible speed
	double relaxation_time;		//time at which a pedestrian takes to get back to desired speed

	bool at_destination;
	
	//sight attributes
	double sight_distance;		//how far an agent can see in metres
	double max_fov; 		//from centre, maximum field of view, in degrees
	double inc_angle;		//how much to vary the angle of the radar
	
	double collision_distance;	//how far person will look ahead to avoid people	//NOT CURRENTLY USED

	double comfort_wall_distance;	//comfortable distance person tries to stay away from walls
	double wall_inc_angle;		//how much to vary the ray when scanning for nearby walls

	double person_sight_distance;	//how far to look for people
	double person_comfort_distance;	//comfortable distance person tries to stay away from people
	double person_inc_angle;	//how much to increment ray angle when scanning
	double max_veer_angle;		//maximum angle person will divert from course to avoid other people

	double collision_inc_angle;	//how much to vary the angle when searching for nearby people
		
};
