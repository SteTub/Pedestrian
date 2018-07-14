#include"Agent_par.h"

//given a list of agents and a width from a chosen border, outputs a list of agents at border
void gatherBorderAgents(std::vector<Person> agents, node *root, std::vector<agent_context> &border_agents, int border, double width, int depth)
{
	int x_dim = depth % 2;
	//border = 0 UP		
	//border = 1 RIGHT		
	//border = 2 DOWN		
	//border = 3 LEFT

	if(root){
			
		//----UP BOUNDARY----//
		if(border == 0){
	
			//if distance is within range, then add to vector
			if(agents[root->index].yPos() >= width)
				border_agents.push_back(packPerson(agents[root->index])); 
			//if in y dimension
			if(x_dim == 1){

				//if agent position is not within range, can then ignore subtree to the left
				if(agents[root->index].yPos() < width){
					gatherBorderAgents(agents, root->right, border_agents, border, width, depth+1);
					return;
				}
			}
		
			//if within range or looking at X dimension, need to check both child nodes			
			gatherBorderAgents(agents, root->right, border_agents, border, width, depth+1);
			gatherBorderAgents(agents, root->left, border_agents, border, width, depth+1);
		}
		
		//----RIGHT BOUNDARY----//
		else if(border == 1){

			//if distance is within range, then add to vector
			if(agents[root->index].xPos() >= width)
				border_agents.push_back(packPerson(agents[root->index])); 
	
			//if in x dimension
			if(x_dim == 0){
	
				//if agent position is not within range, can then ignore subtree to the left
				if(agents[root->index].xPos() < width){
					gatherBorderAgents(agents, root->right, border_agents, border, width, depth+1);
					return;
				}
			}
			
			//if within range or looking at y dimension, need to check both child nodes			
			gatherBorderAgents(agents, root->right, border_agents, border, width, depth+1);
			gatherBorderAgents(agents, root->left, border_agents, border, width, depth+1);
		}
		//----DOWN BOUNDARY----//
		else if(border == 2){

			//if distance is within range, then add to vector
			if(agents[root->index].yPos() <= width)
				border_agents.push_back(packPerson(agents[root->index])); 
	
			//if in Y dimension
			if(x_dim == 1){
	
				//if agent position is not within range, can then ignore subtree to the left
				if(agents[root->index].yPos() > width){
					gatherBorderAgents(agents, root->left, border_agents, border, width, depth+1);
					return;
				}
			}
			
			//if within range or looking at y dimension, need to check both child nodes			
			gatherBorderAgents(agents, root->right, border_agents, border, width, depth+1);
			gatherBorderAgents(agents, root->left, border_agents, border, width, depth+1);
		}
		
		//----LEFT BOUNDARY----//
		else if(border == 3){

			//if distance is within range, then add to vector
			if(agents[root->index].xPos() <= width)
				border_agents.push_back(packPerson(agents[root->index])); 
	
			//if in x dimension
			if(x_dim == 0){
	
				//if agent position is not within range, can then ignore subtree to the left
				if(agents[root->index].xPos() > width){
					gatherBorderAgents(agents, root->left, border_agents, border, width, depth+1);
					return;
				}
			}
			
			//if within range or looking at y dimension, need to check both child nodes			
			gatherBorderAgents(agents, root->right, border_agents, border, width, depth+1);
			gatherBorderAgents(agents, root->left, border_agents, border, width, depth+1);
		}
	}
}

//Function to create a person using context struct
Person ConstructPerson(agent_context person_in){
	
	position start;
	coords ending;
	start.x = person_in.x_pos;
	start.y = person_in.y_pos;
	ending.x = person_in.x_dest;
	ending.y = person_in.y_dest;

	Person output(person_in.id,start,ending,person_in.person_size,person_in.current_speed,person_in.preferred_speed,person_in.max_speed,person_in.sight_distance,MAX_FOV,person_in.comfort_wall_distance,person_in.person_comfort_distance);

	output.setCurrentDestination(person_in.target_orientation, person_in.current_dest, person_in.current_goalpoint, person_in.current_goal_distance, person_in.turn_corner_left);
	output.setOrientation(person_in.orientation);	

	return output;
}

//function creates a struct containing the private members of the class
agent_context packPerson(Person input){
	
	agent_context output;

	output.x_pos 			= input.x_pos;
	output.y_pos 			= input.y_pos;
	output.x_dest 			= input.x_dest;
	output.y_dest 			= input.y_dest;
	output.orientation 		= input.orientation;
	output.target_orientation	= input.target_orientation;
	output.current_dest		= input.current_dest;		
	output.current_goalpoint 	= input.current_goalpoint;	
	output.current_goal_distance 	= input.current_goal_distance;	
	output.turn_corner_left 	= input.turn_corner_left;		
	output.id 			= input.id;				
	output.person_size 		= input.person_size;		
	output.current_speed 		= input.current_speed;	
	output.preferred_speed 		= input.preferred_speed;	
	output.max_speed 		= input.max_speed;		
	output.sight_distance 		= input.sight_distance;		
	output.comfort_wall_distance 	= input.comfort_wall_distance;
	output.person_comfort_distance 	= input.person_comfort_distance;

	return output;
}

//---Default Constructor---//
Person::Person()
{
	//the following will all be varing depending on agent, but kept static for now---//
	id = 0;
	x_pos = 0;		
	y_pos = 0;
	x_dest = 300;
	y_dest = 300;

	orientation = 0;
	goal_angle = atan2((x_dest - x_pos),(y_dest - y_pos))*180/PI;	//goal angle
	if(goal_angle < 0)						//keeps angle between 0 and 360
		goal_angle += 360;

	max_fov = 90;

	at_destination = false;

	//speed
	current_speed = 0;
	preferred_speed = 1.4;			//1.4 m/s 
	//max_speed = 				//no current max speed
	relaxation_time = 0.5;

	person_size = 0.2;		//currently just a general number but defines size of person

	removed = false;

	comfort_wall_distance = 1;
	wall_inc_angle = (180*BOX_SIZE)/(PI*comfort_wall_distance);		//angle given by arc of circle with length BOX_SIZE

	person_sight_distance = 5; 
	person_inc_angle = atan((2*person_size)/person_sight_distance)*180/PI;
	
	current_dest = false;


}

//---Parameterized Constructor---// 
Person::Person(int id, position start, coords destination, double in_person_size, double in_current_speed, double in_preferred_speed, double in_max_speed, double in_sight_distance, double in_max_fov, double in_comfort_wall_distance, double in_person_comfort_distance):
	x_pos{start.x},
	y_pos{start.y},
	x_dest{(double)destination.x},
	y_dest{(double)destination.y},
	id{id},
	person_size{in_person_size},
	current_speed{in_current_speed},
	preferred_speed{in_preferred_speed},
	max_speed{in_max_speed},
	sight_distance{in_sight_distance},
	max_fov{in_max_fov},
	comfort_wall_distance{in_comfort_wall_distance},
	person_comfort_distance{in_person_comfort_distance}
	{
		//ORIENTATIONS/ANGLES
		goal_angle = atan2((x_dest - x_pos),(y_dest - y_pos))*180/PI;	
		if(goal_angle < 0)
			goal_angle += 360;
		orientation = goal_angle;
		
		//PERSONAL ATTRIBUTES
		relaxation_time = 0.5;
	
		removed = false;
	
		//SIGHT DISTANCES/ANGLES
		inc_angle = (180*BOX_SIZE)/(PI*sight_distance)/2;
		
		wall_inc_angle = (180*BOX_SIZE)/(PI*comfort_wall_distance);
	
		at_destination = false;
		current_dest = false;

	}
	
//---Copy Constructor---//
Person::Person(const Person& person_in) :
	x_pos{person_in.x_pos},
	y_pos{person_in.y_pos},
	x_dest{person_in.x_dest},
	y_dest{person_in.y_dest},
	orientation{person_in.orientation},
	target_orientation{person_in.target_orientation},
	goal_angle{person_in.goal_angle},
	current_dest{person_in.current_dest},
	current_goal_distance{person_in.current_goal_distance},
	turn_corner_left{person_in.turn_corner_left},
	id{person_in.id},
	person_size{person_in.person_size},
	current_speed{person_in.current_speed},
	preferred_speed{person_in.preferred_speed},
	max_speed{person_in.max_speed},
	relaxation_time{person_in.relaxation_time},
	at_destination{person_in.at_destination},
	sight_distance{person_in.sight_distance},
	max_fov{person_in.max_fov},
	inc_angle{person_in.inc_angle},
	comfort_wall_distance{person_in.comfort_wall_distance},
	wall_inc_angle{person_in.wall_inc_angle},
	person_comfort_distance{person_in.person_comfort_distance}
{
	removed = false;
	at_destination = false;
	current_goalpoint.x = person_in.current_goalpoint.x;
	current_goalpoint.y = person_in.current_goalpoint.y;

}

//---Destructor---//
Person::~Person() {}

//---ALLOW PRIVATE ELEMENTS TO PUBLIC---//

//---return current x and y positions	
double Person::myID() const{
	return id;
}
double Person::xPos() const{
	return x_pos;
}
double Person::yPos() const{
	return y_pos;
}
//return size of person
double Person::mySize() const{
	return person_size;
}
double Person::mySpeed() const{
	return current_speed;
}
double Person::myOrientation() const{
	return orientation;
}
bool Person::isRemoved() const{
	return removed;
}
/*
//----RETURN THE HISTORY----//		//FOR GRAPHICS
double Person::xHist(int i) const{	//return the history
	
	if((int)x_history.size() < i)
		return 0;

	return x_history[i];
}
double Person::yHist(int i) const{
	
	if((int)y_history.size() < i)
		return 0;

	return y_history[i];
}
int Person::HistSize(){			//return size of history
	return x_history.size();
}
*/
//----MODIFY PRIVATE ELEMENTS----//

void Person::setRemovedTrue(){
	removed = true;
}

//set starting point
void Person::setInitialPosition(coords start){
	x_pos = start.x;
	y_pos = start.y;

}
//set destination point
void Person::setDestination(coords ending){
	x_dest = ending.x;
	y_dest = ending.y;
}
//sets maximum sight distance and also increment angle for radar vision
void Person::setSightDistance(double distance){

	sight_distance = distance;
	inc_angle = (180*BOX_SIZE)/(PI*sight_distance);		//angle given by arc of circle with length BOX_SIZE

	inc_angle/=2;			//final increment is halved to make certain every box will be visible
}

void Person::setOrientation(double orient){
	orientation = orient;
}
void Person::setInitialOrientation(){
	orientation = goal_angle;		//initial orientation is set to be the goal angle, initially starts viewing its destination
}

void Person::setID(int input){

	id = input;
}


//updates the current angle to the goal
void Person::updateGoalAngle(){

	goal_angle = atan2((x_dest - x_pos),(y_dest - y_pos))*180/PI;	
	
	if(goal_angle < 0)			//this keeps the angle between 0 and 360
		goal_angle += 360;		
}
	
//modify the internal members for CORNER DESTINATION
void Person::setCurrentDestination(double target_in, bool current_dest_in, coords current_goalpoint_in, double goal_distance_in, bool turn_corner_left_in){

	target_orientation = target_in;
	current_dest = current_dest_in;
	current_goalpoint = current_goalpoint_in;
	current_goal_distance = goal_distance_in;
	turn_corner_left = turn_corner_left_in;
}
//check if person has reached destination, currently destination is just a point
int Person::isAtDestination(){

	//if current position is end goal return success
	if((int)x_pos == x_dest && (int)y_pos == y_dest){

		at_destination = true;	
//		std::cout << "AGENT[" << id << "] at destination\n";
		return 1;
	}

	return 0;
}
//return whether the agent reached its goal
bool Person::reachedDestination(){
	
	return at_destination;
}

//------MOVEMENT AND SIGHT FUNCTIONS START HERE------------//
//---MAKE MOVE---//
void Person::makeMove(grid city, std::vector<Person> agents, node *root, std::vector<agent_context> u_agents, std::vector<agent_context> r_agents, std::vector<agent_context> d_agents, std::vector<agent_context> l_agents, double u_border, double r_border, double d_border, double l_border){

	if(at_destination == true){
		//do nothing
		return;
	}

	else{
/* PRINT CHECK */
//		std::cout << "---------------\n";
//		std::cout << " Making a move\n";
//		std::cout << "---------------\n";
/* PRINT CHECK */

		//keep orientation between 0 and 360 at every turn
		while(orientation > 360)
			orientation -= 360;	
		if(orientation < 0)
			orientation += 360;	

		//---NAVIGATION---//
		findTargetAngle(city);			//scan environment and get a target orientation
		updateVelocity(city, agents, root, u_agents, r_agents, d_agents, l_agents, u_border, r_border, d_border, l_border);	//update velocity based on current speed, orientation and target orientation
		moveForward();				//update x and y positions
		updateGoalAngle();			//update goal angle based on current position
		if(current_dest){			//if going to a corner
			current_goal_distance -= current_speed*TIME_STEP;	//reduce distance to current target	
		}

/* PRINT CHECK */
//		if(x_pos > 0 && y_pos > 0)	
//			std::cout << "Position = (" << x_pos << ", " << y_pos << ") = [" << city.location[(int)y_pos][(int)x_pos] << "]\n";
//		std::cout << "NEW ORIENTATION = " << orientation << std::endl;
//		std::cout << "NEW GOAL ANGLE = " << goal_angle << std::endl;
/* PRINT CHECK */
		
		if(isAtDestination()){		//if destination is reached	
			x_pos = -1;		//set x and y to negative values to be ignored by other agents so others can reach destination
			y_pos = -1;
			at_destination = true;	
		}
	}

}
//finds the best location given the environment only, sets as current target location
void Person::findTargetAngle(grid city){
	
	//if agent currently has a destination, ie toward a corner
	if(current_dest){
	
		if(current_goal_distance < 0){			//if agent has reached destination
			
			paths corner_length;
			double temp_orientation = orientation;	//temporary orientation to change distance
			double previous_ray_length = 0;		//length of previous sight distance
	
			current_dest = false;			//no longer has a current destination
	
			//when reached the corner, turn based on which way corner turned
			//then find longest route after the turn and face that way
			
			corner_length = rayCast(city,temp_orientation,sight_distance);	
	
			while(fabs(corner_length.probable_path) > fabs(previous_ray_length)){	//find longest path

				if(turn_corner_left == false)			//if want to rotate clockwise (turn right)		
					temp_orientation += inc_angle;
				else temp_orientation -= inc_angle;		//if want to rotate anti-clockwise (turn left)			
				previous_ray_length = corner_length.probable_path;
				corner_length = rayCast(city,temp_orientation,sight_distance);	
			
			}		

			target_orientation = temp_orientation;		//set new orientation

		}
		//if destination hasnt been reached
		else{							
			target_orientation = atan2((current_goalpoint.x - x_pos),(current_goalpoint.y - y_pos))*180/PI;	//angle toward current destination
			if(target_orientation < 0)
				target_orientation += 360;

			//orientation = target_orientation;	
		}
	}

	//no current destination => scans for new goal
	else{	
		double new_orientation = radarVision(city);

		//if no available options, turns around
		if(new_orientation == 1000){
	
			new_orientation += 180;
			//orientation += 180;
			//new_orientation = radarVision(city, x_radar, y_radar);
		}
	
		target_orientation = new_orientation;		
	}
}

//update velocity using social force model
void Person::updateVelocity(grid city, std::vector<Person> agents, node *root, std::vector<agent_context> u_agents, std::vector<agent_context> r_agents, std::vector<agent_context> d_agents, std::vector<agent_context> l_agents, double u_border, double r_border, double d_border, double l_border){

	//vector coordinates of current vector
	double x_1 = current_speed*sin(orientation*PI/180);		
	double y_1 = current_speed*cos(orientation*PI/180);	
	//vector coordinates of target vector
	double x_2 = preferred_speed*sin(target_orientation*PI/180);	
	double y_2 = preferred_speed*cos(target_orientation*PI/180);

	double x_3 = x_2 - x_1;						
	double y_3 = y_2 - y_1;
	
	//change in velocity due to deviation from preferred velocity	
	double delta_v_X = TIME_STEP*x_3/relaxation_time;
	double delta_v_Y = TIME_STEP*y_3/relaxation_time;

	//---UPDATE VELOCITY TO TAKE INTO ACCOUNT NEAREST WALL---//
	location_info closest_wall;		//vector of a nearby wall, angle and distance
	
	//strength and range parameters
	double a=15;			
	double b=0.2;			
	double x_wall, y_wall, wall_strength;	
	
	for(int i=0;i<2;i++){
		//find the walls nearby	to left
		closest_wall = findNearestWall(city, i);
	
		if(closest_wall.distance == 2*comfort_wall_distance){	//if wall is outside interaction range then it ignores
			wall_strength = 0;				//strength of wall interaction becomes zero
		}
		else wall_strength = a*exp(-1*closest_wall.distance/b);	//exponentially decreasing potential
	
		//vector of nearest wall
		x_wall = TIME_STEP*wall_strength*sin(closest_wall.angle*PI/180);		
		y_wall = TIME_STEP*wall_strength*cos(closest_wall.angle*PI/180);	

		delta_v_X += x_wall;
		delta_v_Y += y_wall;

	}

	//---UPDATE VELOCITY TO AVOID COLLISIONS WITH PEOPLE---//
	std::vector<int> kNN_index;
	findNeighbours(root, 0, agents, kNN_index);

	position person_vector = avoidPeople(agents, kNN_index, u_agents, r_agents, d_agents, l_agents, u_border, r_border, d_border, l_border);	

	//new velocity vector 
	double new_X = x_1 + delta_v_X + person_vector.x;
	double new_Y = y_1 + delta_v_Y + person_vector.y;

	//convert to polar coordinates
	current_speed = sqrt((new_X)*(new_X) + (new_Y)*(new_Y));
	if(current_speed > max_speed)
		current_speed = max_speed;

	orientation = atan2(new_X, new_Y) * 180/PI;
	if(orientation < 0)
		orientation += 360;
	
}

//finds nearest wall, returns angle to wall and distance to wall
location_info Person::findNearestWall(grid city, int on_right){

	paths wall_ray;						//use to probe output of raycast
	location_info closest_wall;				//return the distance to wall and angle
	closest_wall.distance = comfort_wall_distance*2;	//distance to nearest wall
	closest_wall.angle = 0;

	double actual_x_pos = x_pos;	//need to save current position since they are changed for the next check
	double actual_y_pos = y_pos;	

	moveForward();	//this will update x_pos and y_pos into the positions they would be if agent moves towards current choice 

	double start_orient, end_orient;

	if(on_right == 1){	//search for nearest wall to right
		start_orient = orientation;
		end_orient = orientation + 90;	
	}
	else{			//search for nearest wall on left
		start_orient = orientation - 90;
		end_orient = orientation;	
	}

	for(double location_angle = start_orient; location_angle < end_orient; location_angle += wall_inc_angle){
		
		//search for wall
		wall_ray = rayCast(city,location_angle,comfort_wall_distance);	
	
		if(wall_ray.open_path == 1){	//if wall is found
			
			//if current wall is closer than previous closest
			if(closest_wall.distance > wall_ray.probable_path){	
				//select current wall as closest
				//rotate angle by 180 to get direction from wall to person
				closest_wall.angle = location_angle - 180;	
				closest_wall.distance = wall_ray.probable_path - person_size;
			}	
		}
	}
	
	if(closest_wall.angle < 0)		//keep value positive
		closest_wall.angle += 360;

	x_pos = actual_x_pos;			//reset x and y positions to actual positions
	y_pos = actual_y_pos;

	return closest_wall;			//return the vector to the nearest wall

}

//----find neighbours----//
//creates a vector of indexes of nearby agents
void Person::findNeighbours(node *root, int depth, std::vector<Person> agents, std::vector<int> &kNN_index){

	int x_dim = depth % 2;

	if(root){

		if((agents[root->index].xPos() - x_pos)*(agents[root->index].xPos() - x_pos) + (agents[root->index].yPos() - y_pos)*(agents[root->index].yPos() - y_pos) <= person_comfort_distance*person_comfort_distance)
			kNN_index.push_back(root->index); 

		//if in x dimension
		if(x_dim == 0){

			//if position of search point is strictly to the right of current node, can then ignore subtree to the left
			if(x_pos - agents[root->index].xPos() > PERSON_COMFORT_DIST){
				findNeighbours(root->right, depth+1, agents, kNN_index);
				return;
			}
			//if position of search point is strictly to the left of current node, can then ignore subtree to the right
			if(agents[root->index].xPos() - x_pos > PERSON_COMFORT_DIST){
				findNeighbours(root->left, depth+1, agents, kNN_index);
				return;
			}
		}
		//if in y dimension
		else{

			if(y_pos - agents[root->index].yPos() > PERSON_COMFORT_DIST){
				findNeighbours(root->right, depth+1, agents, kNN_index);
				return;
			}
			if(agents[root->index].yPos() - y_pos > PERSON_COMFORT_DIST){
				findNeighbours(root->left, depth+1, agents, kNN_index);
				return;
			}
		}
		//if there can be neighbours to either side then check both			
		findNeighbours(root->right, depth+1, agents, kNN_index);
		findNeighbours(root->left, depth+1, agents, kNN_index);
	}
}

//----SOCIAL FORCE MODEL CALCULATIONS----//

position Person::calculateSocialForce(double other_x_pos, double other_y_pos, double other_speed, double other_orientation, double other_size)
{
	position output;
	output.x = 0;
	output.y = 0;
	double x_after_move = other_x_pos + other_speed*TIME_STEP*sin(other_orientation*PI/180); 
	double y_after_move = other_y_pos + other_speed*TIME_STEP*cos(other_orientation*PI/180); 

	//distance away from person
	double person_distance = sqrt((x_after_move - x_pos)*(x_after_move - x_pos) + (y_after_move - y_pos)*(y_after_move - y_pos)) - (person_size + other_size);	//distance between centres reduced by width of people

	double person_angle = atan2((x_after_move - x_pos),(y_after_move - y_pos))*180/PI;
	//if(person_angle < 0)						
	//	person_angle += 180;
	//else person_angle -= 180;
	person_angle -= 180;	
				
	double a = 2;						//strength of interaction
	double b = 0.3;			
	if(person_distance < person_comfort_distance){		//if person is within interaction range then it ignores
	
		double person_force = a*exp(-1*person_distance/b);	//exponentially decreasing potential
		
		//vector of forces from nearby people
		output.x += TIME_STEP*person_force*sin(person_angle*PI/180);		
		output.y += TIME_STEP*person_force*cos(person_angle*PI/180);	
	}


	return output;
}

//use social force model to avoid people
position Person::avoidPeople(std::vector<Person> agents, std::vector<int> kNN_index, std::vector<agent_context> u_border_agents, std::vector<agent_context> r_border_agents, std::vector<agent_context> d_border_agents, std::vector<agent_context> l_border_agents, double left_bound, double right_bound, double up_bound, double down_bound){
//position Person::avoidPeople(std::vector<Person> agents, std::vector<int> kNN_index){			
	
	position output;
	output.x = 0;
	output.y = 0;	

	//iterates through the indexes of the nearby agents
	for(size_t i=0;i<kNN_index.size();i++){
		if(agents[kNN_index[i]].myID() != id){	//if not looking at itself

			output += calculateSocialForce(agents[kNN_index[i]].xPos(), agents[kNN_index[i]].yPos(), agents[kNN_index[i]].mySpeed(), agents[kNN_index[i]].myOrientation(), agents[kNN_index[i]].mySize());
	
		}
	}

	//-----CHECK IF CLOSE TO BORDER-----//
	if(y_pos > up_bound){
		for(size_t i=0;i<u_border_agents.size();i++){
			output += calculateSocialForce(u_border_agents[i].x_pos, u_border_agents[i].y_pos, u_border_agents[i].current_speed, u_border_agents[i].orientation, u_border_agents[i].person_size);
		}
	}			
	else if(y_pos < down_bound){
		for(size_t i=0;i<d_border_agents.size(); i++){

			output += calculateSocialForce(d_border_agents[i].x_pos, d_border_agents[i].y_pos, d_border_agents[i].current_speed, d_border_agents[i].orientation, d_border_agents[i].person_size);
		}
	}			
	if(x_pos > right_bound){
		for(size_t i=0;i<r_border_agents.size(); i++){
			output += calculateSocialForce(r_border_agents[i].x_pos, r_border_agents[i].y_pos, r_border_agents[i].current_speed, r_border_agents[i].orientation, r_border_agents[i].person_size);
		}
	}			
	if(x_pos < left_bound){
		for(size_t i=0;i<l_border_agents.size(); i++){
			output += calculateSocialForce(l_border_agents[i].x_pos, l_border_agents[i].y_pos, l_border_agents[i].current_speed, l_border_agents[i].orientation, l_border_agents[i].person_size);

		}
	}			
	//output.x = x_person;
	//output.y = y_person;

	return output;
}

//update current position
void Person::moveForward(){

	x_pos += current_speed*TIME_STEP*sin(orientation*PI/180);
	y_pos += current_speed*TIME_STEP*cos(orientation*PI/180);
}

//to change orientation by a certain value
/*<<< NOT CURRENTLY USED >>>
void Person::rotate(double theta){

	orientation += theta;
	if(orientation >= 360)
		orientation -= 360;

}
*/

//----OPEN PATH----//
//wall_check determines if open path or if path is close to a wall, 0 if open, 2 if wall 
double Person::checkOpenPath(int open_path, double input_angle, double current_choice, double diff, int wall_check)
{
	if(open_path == wall_check && current_choice == 1000)		//if path is open and not yet chosen
			current_choice = input_angle;		//choose the current angle as the eventual angle of orientation for movement
	else if(open_path == wall_check && fabs(diff - inc_angle) > fabs(diff))	//if open path AND not on first angle AND previous angle is further from goal angle then choose this angle, ie want to be directed toward goal
			current_choice = input_angle;
	
	return current_choice;
}

//----PROBABLE PATH----//
double Person::checkProbablePath(double current_path, double previous_path, double input_angle, double init_angle, double current_choice)
{		
	if(input_angle != init_angle){		//doesn't apply to first ray
	
		double jump = current_path - previous_path;		//jump is difference in lengths between the two rays
	
		if(fabs(jump) > JUMP_CRITERIA){				//if jump is large enough
		
			//if the probable path has already been chosen
			if(current_choice != 1000){	
				if(fabs(input_angle - goal_angle) < fabs(current_choice - goal_angle)){		//if new angle is closer to currently chosen angle
					
					if(jump > 0){	//if open space to right/clockwise
						current_choice = input_angle*-1;	//sign implies corner is to left or right, right => -1
					}
					else {		//if open path on left/anticlockwise
						current_choice = input_angle - inc_angle;
					}
				}	
			}
			//if the probable path has yet to be chosen
			else{
				if(jump > 0){
					current_choice = input_angle*-1;
				}
				else{
					current_choice = input_angle - inc_angle;
				}	
			}
		}	

	}

	return current_choice;
}

//----NAVIGATE CORNER----//
double Person::navigateCorner(grid city, double current_angle){

	bool jump_right;		//if corner opens up to right, jump_right = true;
	int steep = 0;
	double inc = inc_angle;

	if(current_angle < 0){		//corner opens up to right (clockwise)
		jump_right = true;
		inc *= -1;
		current_angle *= -1;
	}
	else jump_right = false;	//corner opens up to left (anti-clockwise)
	
	paths path_1 = rayCast(city, current_angle, sight_distance);
	paths path_2 = rayCast(city, current_angle + inc, sight_distance);	//this will be find the corner on the left or right given by inc
	double corner_distance = (fabs(path_1.probable_path) + fabs(path_2.probable_path))/2;	//distance to midpoint of paths at a corner
//	double corner_distance = fabs(path_1.probable_path) < (path_2.probable_path) ? fabs(path_1.probable_path) : fabs(path_2.probable_path);	//choose shorter of the two distances

	double corner_x = x_pos + corner_distance*sin(current_angle*PI/180);	//position of corner
	double corner_y = y_pos + corner_distance*cos(current_angle*PI/180);

	if(fabs(corner_x - x_pos) < fabs(corner_y - y_pos))		//if slope is steep, ie y>x
		steep = 1;

	coords corner;
	corner.x = (int)corner_x;
	corner.y = (int)corner_y;
	
	//Want to move away from the corner, and head for the nearest open path [0]

	if(jump_right == true){			//if corner on the right
		if(steep){
			while(pathBlocked(city, corner.x, corner.y) != 0){
				if(corner_y > y_pos)		//facing north
					corner.x++;	
				else corner.x--;		//facing south
			}
		}
		else{
			while(pathBlocked(city, corner.x, corner.y) != 0){
				if(corner_x > x_pos){		//facing east
					corner.y--;	
				}	
				else corner.y++;		//facing west
			}
		}
	}	
	else{
		if(steep){
			while(pathBlocked(city, corner.x, corner.y) != 0){
				if(corner_y > y_pos)		//facing north
					corner.x--;	
				else corner.x++;		//facing south
			}
		
		}
		else{
			while(pathBlocked(city, corner.x, corner.y) != 0){
				if(corner_x > x_pos)		//facing east
					corner.y++;	
				else corner.y--;		//facing west
			}
		}
	}

	//now set new orientation using the new endpoint
	double new_angle = atan2((corner.x - x_pos),(corner.y - y_pos))*180/PI;	
	if(new_angle < 0)
		new_angle += 360;

	current_dest = true;				//set that has a current destination
	current_goalpoint = corner; 			//set position of current destination
	if(jump_right == true)
		turn_corner_left = true;		//turn to left after corner
	else turn_corner_left = false;			//turn right after corner

	current_goal_distance = sqrt((corner.x - x_pos)*(corner.x - x_pos) + (corner.y - y_pos)*(corner.y - y_pos));	//distance to destination

	return new_angle;
}

//------RADAR VISION------//
//return direction angle 
double Person::radarVision(grid city){

	paths path_choice;
	double avail_open_angle = 1000;			//1000 implies no available open rays yet
	double avail_probable_angle = 1000;		//1000 implies no available probable paths yet
	//double avail_open_wall_angle = 1000;
	double previous_ray_distance = 0;		//distance of previous ray
	double current_ray_distance;			//distance of next ray

	//angles at each side of radar
	double init_angle = orientation - max_fov;
	double final_angle = orientation + max_fov;
	double positive_init_angle = init_angle < 0 ? init_angle + 360 : init_angle;

	double diff = positive_init_angle - goal_angle;			//this keeps track of the difference in angle between current ray and goal angle
	//only want inner angle between rays
	if(diff > 180)
		diff -= 360;
	else if(diff < -180)
		diff += 360;

	//loops through until actual orientation is reached
	for(double view_angle = init_angle; view_angle < orientation; view_angle += inc_angle){		
		
		//----RAY CAST---//
		path_choice = rayCast(city,view_angle, sight_distance);

		//----OPEN PATH----//
		avail_open_angle = checkOpenPath(path_choice.open_path, view_angle, avail_open_angle, diff, 0);
	
		diff+=inc_angle;	//update angle between goal and current ray angle

		//----PROBABLE PATH----//
		previous_ray_distance = current_ray_distance;
		current_ray_distance = path_choice.probable_path; 

		avail_probable_angle = checkProbablePath(current_ray_distance, previous_ray_distance, view_angle, init_angle, avail_probable_angle);		
		//----OPEN PATH NEXT TO WALL----//
		//avail_open_wall_angle = checkOpenPath(path_choice.open_path, view_angle, avail_open_wall_angle, diff, 2);
	}

	//reset diff using current orientation
	positive_init_angle = orientation < 0 ? orientation + 360 : orientation;
	diff = positive_init_angle - goal_angle;
	//keep difference as inner angle
	if(diff > 180){
		diff -= 360;
	}
	else if(diff < -180)
		diff += 360;

	//loops from actual orientation to end
	for(double view_angle = orientation; view_angle < final_angle; view_angle += inc_angle){
		
		path_choice = rayCast(city,view_angle, sight_distance);
	
		//----OPEN PATH----//	
		avail_open_angle = checkOpenPath(path_choice.open_path, view_angle, avail_open_angle, diff, 0);

		diff+=inc_angle;	//update angle between goal and current ray angle
		
		//----PROBABLE PATH----//
		previous_ray_distance = current_ray_distance;
		current_ray_distance = path_choice.probable_path; 
		
		avail_probable_angle = checkProbablePath(current_ray_distance, previous_ray_distance, view_angle, init_angle, avail_probable_angle);		
	}

	//---finally raycast at final position---//
	path_choice = rayCast(city,final_angle, sight_distance);

	//----OPEN PATH----//	
	avail_open_angle = checkOpenPath(path_choice.open_path, final_angle, avail_open_angle, diff, 0);
		
	//----PROBABLE PATH----//
	previous_ray_distance = current_ray_distance;
	current_ray_distance = path_choice.probable_path; 

	avail_probable_angle = checkProbablePath(current_ray_distance, previous_ray_distance, final_angle, init_angle, avail_probable_angle);	
	if(avail_open_angle < 0)
		avail_open_angle += 360;
	else if(avail_open_angle != 1000 && avail_open_angle > 360)	
			avail_open_angle -= 360;

	//---RETURN THE FINAL DECISION----//	
	if(avail_open_angle != 1000)		//if there is an available open path
		return avail_open_angle;
	else if(avail_probable_angle != 1000){	//if there is an available probable path

		double corner_angle = navigateCorner(city, avail_probable_angle);
		return corner_angle;
	}
//	else if(avail_open_wall_angle != 1000)
//		return avail_open_wall_angle;	
	
	return 1000;				//if nothing available, returns a failed result (1000 = no route available)
}

//-----RAY CAST ALGORITHM-----//
//the starting and end points are rounded down to integers

//output is whether the path is open (open_path) and the length of the path (probable_path)
paths Person::rayCast(grid city, double ray_angle, double view_distance){

	paths output;
	output.open_path = 0;
	output.probable_path = 0;
 
	int goal = 0;
	double x2, y2;	
	double init_gapX, init_gapY;
	double gapX, gapY;
	int incX=1, incY=1;

	//starting point is the coordinate the person is standing in, rounded down to nearest integer
	int x1 = (int)x_pos;
	int y1 = (int)y_pos;
	
	//if value of starting point is negative, this takes into account that -1.5 should go to -2 
	if(x1 < 0)
		x1--;	
	if(y1 < 0)
		y1--;

	//keeps ray angle from being negative
	if(ray_angle < 0)
		ray_angle += 360;

	//Calculates the position of (x2,y2) endpoint using angle from starting point	
	x2 = x1 + (view_distance/city.box_width)*sin(ray_angle*PI/180);
	y2 = y1 + (view_distance/city.box_width)*cos(ray_angle*PI/180);
	
	//if value of endpoint was negative, moves to correct integer 
	if(x2 < 0)
		--x2;
	if(y2 < 0)
		--y2;

	//Set x or y to zero if in direction of N.S.E.W
	//floating point number can be problematic when very small
	if(ray_angle == 0 || ray_angle == 180)
		x2 = x1;
	else if(ray_angle == 90 || ray_angle == 270)
		y2 = y1;

	//if the direction of the ray is negative, increment becomes decrement
	if(x2 < x1)
		incX = -1;
	if(y2 < y1)
		incY = -1;

	//----NEW ANGLE BETWEEN INTEGER ENDPOINT AND INTEGER STARTING POINT----//
	//the old point will have a slightly different angle
	double new_angle = atan2(((int)x2-x1),((int)y2-y1));

	//make this value positive for sin/cos functions	
	if(new_angle < 0)
		new_angle += 2*PI;

	gapX = std::abs(city.box_width/sin(new_angle));
	gapY = std::abs(city.box_height/cos(new_angle));

	//set initial value which will be updated
	init_gapX = gapX;
	init_gapY = gapY;

/* PRINT CHECK */
//	std::cout << "NEW (" << x2 << ", " << y2 << ")\n";
/* PRINT CHECK */
//	std::cout << "<<<<----WHAT I SEE [" << city.location[y1][x1] << "]\n";
/* PRINT CHECK */

	//loop through values until endpoint is reached
	while(x1 != (int)x2 || y1 != (int)y2){

		if(init_gapX < init_gapY){	//if ray is closer to an X coordinate than a Y coordinate
			init_gapX += gapX;	//update distance to next X position
			x1+=incX;		//increase X position
		}
		else{				
			init_gapY += gapY;
			y1+=incY;
		}

		if(x1 > 0 && y1 > 0){
			if(x1 == (int)x_dest && y1 == (int)y_dest){		//if reached destination
				output.open_path = 0;				//return an open path if found destination
				goal = 1;
				break;						//stop scanning
			}
			else if(pathBlocked(city,x1,y1) == 1){	//use x,y value to determine if path is blocked
				output.open_path = 1; 		//if position is blocked, path is not open, open path = 0
				break;				//stop when path is blocked
			}
			//check if path is next to a wall 	
			else if(pathBlocked(city,x1,y1) == 2){
				output.open_path = 2;
			}
		}
		else {
		//	std::cout << "(" << x1 << ", " << y1 << ")\n";
			output.open_path = 1;
			break;
		}	
/* PRINT CHECK */
//		std::cout << "<<<<----WHAT I SEE [" << city.location[y1][x1] << "]\n";
/* PRINT CHECK */
	
/* PRINT CHECK */
//		std::cout << "(" << x1 << ", " << y1 << ")\n";
/* PRINT CHECK */
	}
	//for PROBABLE PATH the output is simply the distance of the ray	
	output.probable_path = sqrt((BOX_SQUARE)*((x1-(int)x_pos)*(x1-(int)x_pos) + (y1-(int)y_pos)*(y1-(int)y_pos)));

	if(goal)
		output.probable_path = sight_distance + 10;	//if sees exit, wants to prioritize as the 'longest' path
	
	return output;
}
/*
//-----RAY CAST ALGORITHM-----//
//the starting and end points are rounded down to integers
//USE FOR GRAPHICS
void Person::rayCastGraphic(grid city, double ray_angle, std::vector<int> &x_radar, std::vector<int> &y_radar){

	double x2, y2;	
	double init_gapX, init_gapY;
	double gapX, gapY;
	int incX=1, incY=1;

	//starting point is the cordinate the person is standing in, rounded to nearest integer
	int x1 = (int)x_pos;
	int y1 = (int)y_pos;

	//set precision of output for checking purposes
//	std::cout.precision(17);
	
	//if value of starting point is negative, this takes into account that -1.5 should go to -2 
	if(x1 < 0)
		x1--;	
	if(y1 < 0)
		y1--;

	//keeps ray angle from being negative
	if(ray_angle < 0)
		ray_angle += 360;

	//Calculates the position of (x2,y2) endpoint using angle from starting point	
	x2 = x1 + (sight_distance/city.box_width)*sin(ray_angle*PI/180);
	y2 = y1 + (sight_distance/city.box_width)*cos(ray_angle*PI/180);
	
	//if value of endpoint was negative, moves to correct integer 
	if(x2 < 0)
		--x2;
	if(y2 < 0)
		--y2;

	//Set x or y to zero if in direction of N.S.E.W
	//floating point number can be problematic when very small
	if(ray_angle == 0 || ray_angle == 180)
		x2 = x1;
	else if(ray_angle == 90 || ray_angle == 270)
		y2 = y1;

	//if the direction of the ray is negative, increment becomes decrement
	if(x2 < x1)
		incX = -1;
	if(y2 < y1)
		incY = -1;

	//----NEW ANGLE BETWEEN INTEGER ENDPOINT AND INTEGER STARTING POINT----//
	//the old point will have a slightly different angle
	double new_angle = atan2(((int)x2-x1),((int)y2-y1));

	//make this value positive for sin/cos functions	
	if(new_angle < 0)
		new_angle += 2*PI;

	gapX = std::abs(city.box_width/sin(new_angle));
	gapY = std::abs(city.box_height/cos(new_angle));

	//set initial value which will be updated
	init_gapX = gapX;
	init_gapY = gapY;

	//add initial values to array
	//x_radar.push_back(x1 - x1);
	//y_radar.push_back(y1 - y1);
	x_radar.push_back(x1);
	y_radar.push_back(y1);

	//loop through values until endpoint is reached
	while(x1 != (int)x2 || y1 != (int)y2){
	
		if(init_gapX < init_gapY){
			init_gapX += gapX;
			x1+=incX;
		}
		else{
			init_gapY += gapY;
			y1+=incY;
		}

		if(x1 > 0 && y1 > 0)
			if(pathBlocked(city,x1,y1) == 1)	//use x,y value to determine if path is blocked
				break;

		else break;
		//add new values to array
	//	x_radar.push_back(x1 - (int)x_pos);
	//	y_radar.push_back(y1 - (int)y_pos);
		x_radar.push_back(x1);
		y_radar.push_back(y1);
	}
}
*/
