//header guard
#ifndef __AGENT_H_INCLUDED__
#define __AGENT_H_INCLUDED__

#include<iostream>
#include<vector>
#include<cmath>
#include"Environment_par.h"
#include"kd_tree.h"

#define ROWS 2000
#define COLUMNS 2000

#define MAX_FOV 90.0
#define SIGHT_DIST 40.0
#define PERSON_SIZE 0.2		
#define PREFERRED_SPEED 1.4
#define COMFORT_WALL_DIST 0.7
#define PERSON_COMFORT_DIST 2.0 


#define JUMP_CRITERIA 3		//requirement for jump criteria

#define TIME_STEP 0.5 		//0.5 of a second 

#define BOX_SIZE 0.5		//0.5 metres
#define BOX_SQUARE BOX_SIZE*BOX_SIZE

#define PI 3.14159265358979323846

//----FORWARD DECLARATIONS----//
struct node;
typedef node node;

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
//struct to contain float vector positions
struct position{
	double x;
	double y;

	position operator+=(const position &addition)
	{
		x+=addition.x;
		y+=addition.y;
		return *this;
	}

};

//individual components of an agent, for sending with MPI
struct agent_context{
	
	int id;				//specific identifier
	
	bool current_dest;		//if true, agent currently has a target destination
	bool turn_corner_left;		//if true, corner is to the left so person should rotate to left to see around corner
	
	double x_pos, y_pos;		//current position
	double x_dest, y_dest;		//position of destination
	double orientation;		//this is the angle the person is facing, in degrees
	double target_orientation;	//desired orientation
	double current_goal_distance;	//distance to current objective

	double person_size;		//size of person, this will be radius of disk
	double current_speed;		//movement distance = speed x timestep
	double preferred_speed;		//preferred personal speed, tries to maintain this
	double max_speed;		//maximum possible speed
	double sight_distance;		//how far an agent can see in metres

	double comfort_wall_distance;	//comfortable distance person tries to stay away from walls
	double person_comfort_distance;	//comfortable distance person tries to stay away from people
	
	coords current_goalpoint;	//coordinates of current goal
};

//---AGENT CLASS---//
class Person {

public:
	//Default constructor
	Person();
	//Constructor 
	Person(int id, position start, coords destination, double in_person_size, double in_current_speed, double in_preferred_speed, double in_max_speed, double in_sight_distance, double in_max_fov, double in_comfort_wall_distance, double in_person_comfort_distance);
	//Copy Constructor
	Person(const Person& person_in);

	//Destructor
	~Person();

	//movement functions
	void makeMove(grid city, std::vector<Person> agents, node *root, std::vector<agent_context> u_agents, std::vector<agent_context> r_agents, std::vector<agent_context> d_agents, std::vector<agent_context> l_agents, double u_border, double r_border, double d_border, double l_border);
	//void makeMove(grid city, std::vector<Person> agents, node *root);
	void findTargetAngle(grid city);	//find desired path, return the angle of the target location
	void updateVelocity(grid city, std::vector<Person> agents, node *root, std::vector<agent_context> u_agents, std::vector<agent_context> r_agents, std::vector<agent_context> d_agents, std::vector<agent_context> l_agents, double u_border, double r_border, double d_border, double l_border);
//	void updateVelocity(grid city, std::vector<Person> agents, node *root);		//updates velocity, includes wall forces
	void moveForward();
	void checkValidMove(grid city);		//NOT USED
	void rotate(double theta);		//NOT USED
	int isAtDestination();			//check if at destination
	bool reachedDestination();

	//vision functions
	double checkOpenPath(int open_path, double input_angle, double current_choice, double diff, int wall_check); //return the direction of the best open path
	double checkProbablePath(double current_path, double previous_path, double input_angle, double init_angle, double current_choice);	//return direction of best probable path
	double navigateCorner(grid city, double current_angle);		//modifies the probable path angle to avoid getting close to corner
	double radarVision(grid city);	//scans through a range of angles using raycasts
	paths rayCast(grid city, double ray_angle, double view_distance);	//send a ray out through the grid in a straight line from current position
//	void rayCastGraphic(grid city, double ray_angle);	//ray cast for graphic
	
	//NOT USED
	//void drawLine(grid city, double ray_angle);

	//collision functions
	void findNeighbours(node *root, int depth, std::vector<Person> agents, std::vector<int> &kNN_index);
//	position avoidPeople(std::vector<Person> agents, std::vector<int> kNN_index);			

// ADDING THIS
	position calculateSocialForce(double other_x_pos, double other_y_pos, double other_speed, double other_orientation, double other_size);
	position avoidPeople(std::vector<Person> agents, std::vector<int> kNN_index, std::vector<agent_context> u_border_agents, std::vector<agent_context> r_border_agents, std::vector<agent_context> d_border_agents, std::vector<agent_context> l_border_agents, double left_bound, double right_bound, double up_bound, double down_bound);

	location_info findNearestWall(grid city, int on_right);		//finds the nearest wall and returns the distance and angle to it

	//functions to access private members
	double myID() const;		//access ID of agent
	double xPos() const;		//access x position
	double yPos() const;		//access y position
	double mySpeed() const;
	double myOrientation() const;
	bool isRemoved() const;		//check whether agent has been removed

	double xHist(int i) const;	//access history at point 'i'
	double yHist(int i) const;
	int HistSize();			//size of history
	double mySize() const;		//person size
	
	//functions to update private members		
	void setID(int input);
	void setRemovedTrue();
	void setInitialPosition(coords start);		//set starting coordinates
	void setDestination(coords ending);		//set destination coords
	void setSightDistance(double distance);		//set maximum sight distance
	void setOrientation(double orientation);	//modify orientation
	void setInitialOrientation();			//set initial orientation
	void updateGoalAngle();				//reset angle to destination using current orientation
	void setCurrentDestination(double target_in, bool current_dest_in, coords current_goalpoint_in, double goal_distance_in, bool turn_corner_left_in);

	friend agent_context packPerson(Person input);
	
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
//	std::vector<double> x_history;		//FOR GRAPHICS
//	std::vector<double> y_history;		//FOR GRAPHICS

	//Personal attributes
	int id;				//specific identifier
	double person_size;		//size of person, this will be radius of disk
	double current_speed;		//movement distance = speed x timestep
	double preferred_speed;		//preferred personal speed, tries to maintain this
	double max_speed;		//maximum possible speed
	double relaxation_time;		//time at which a pedestrian takes to get back to desired speed

	bool removed;
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
	double max_veer_angle;		//maximum angle person will divert from course to avoid other people	//NOT CURRENTLY USED

	double collision_inc_angle;	//how much to vary the angle when searching for nearby people
		
};

//NON CLASS FUNCTION
Person ConstructPerson(agent_context person_in);
agent_context packPerson(Person input);
void gatherBorderAgents(std::vector<Person> agents, node *root, std::vector<agent_context> &border_agents, int border, double width, int depth);

#endif
