//#include "Agent_par.h"
#include "kd_tree.h"

//----CREATE A NEW NODE----//
node* newNode(int agent_index, int agent_id)	//create with parameters
{
	node* temp = new node;

	//set with given parameters
	temp->index = agent_index;		
	temp->id = agent_id;			
	temp->left = temp->right = NULL;	//no child nodes

	return temp;
}

//----RECURSIVELY INSERT A POINT----//
node *insertRec(node *root, int insert_index, std::vector<Person> agents, int depth)
{
	//if at an empty node, create node at this point
	if(root == NULL)
		return newNode(insert_index, agents[insert_index].myID());

	int x_dim = depth % 2;		//determine if splitting along x or y

	if(x_dim == 0)			//if x split 
	{
		//if value to be inserted is less than value at current node, insert to the left
		if(agents[insert_index].xPos() < agents[(root->index)].xPos())			
			root->left = insertRec(root->left, insert_index, agents, depth+1);
		//otherwise, insert to the right
		else 	
			root->right = insertRec(root->right, insert_index, agents, depth+1);
	}
	else	//same as above, for y dimension in this case
	{	
		if(agents[insert_index].yPos() < agents[(root->index)].yPos())
			root->left = insertRec(root->left, insert_index, agents, depth+1);
		else 
			root->right = insertRec(root->right, insert_index, agents, depth+1);
	}

	return root;
}

//----INSERT INDEX OF AGENT----//
node* insert(node *root, int insert_index, std::vector<Person> agents)
{
	return insertRec(root, insert_index, agents, 0);
}

//----RETURN THE MINIMUM OF THREE NODES----//
node *minNode(node *x, node *y, node *z, int x_dim, std::vector<Person> agents)
{
	node *output = x;	//output this node

	if(x_dim == 0){		//minimum in x dimension
		if(y != NULL && agents[y->index].xPos() < agents[output->index].xPos())
			output = y;
		if(z != NULL && agents[z->index].xPos() < agents[output->index].xPos())
			output = z;
	}
	
	if(x_dim == 0){		//minimum in y dimension	
		if(y != NULL && agents[y->index].yPos() < agents[output->index].yPos())
			output = y;
		if(z != NULL && agents[z->index].yPos() < agents[output->index].yPos())
			output = z;
	}

	return output;
}

//----RECURSIVELY MOVES THROUGH THE TREE UNTIL THE MINIMUM VALUE IS FOUND----//
node *findMinRec(node *root, int x_dim, int depth, std::vector<Person> agents)
{
	if(root == NULL)		//if node doesnt exist
		return NULL;

	int dim = depth % 2;		//determines which dimension to consider
	
	if(dim == x_dim)		//if dimension of current node matched the search dimension
	{
		if(root->left == NULL)					//if no further left tree	
			return root;					//return left most node
		return findMinRec(root->left, x_dim, depth+1, agents);	//else proceed further down the left subtree
	}

	//if dimension at this point is different, then need to check all points around this one 
	return minNode(root, findMinRec(root->left, x_dim, depth+1, agents), findMinRec(root->right, x_dim, depth+1, agents), x_dim, agents);

}

//----FIND MINIMUM NODE OF SELECTED DIMENSION----//
node *findMin(node* root, int x_dim, std::vector<Person> agents)
{
	return findMinRec(root, x_dim, 0, agents);
}

//----RECURSIVELY DELETE NODE FOR AGENT(ID)----//
node* deleteNodeRec(node *root, int id, int index, int depth, std::vector<Person> agents)
{
	if(root == NULL)		//if node is not there, do nothing
		return NULL;		

	int x_dim = depth % 2;		//change cutting dimension
	
	if(agents.size() < 1)		//do nothing if there are no agents
		return NULL;

	if(index > (int)agents.size())	//if looking for non exitent agent
		return NULL;

	//if current node contains the search ID
	if(root->id == id){			

		if(root->right != NULL){					//if node has a right subtree

			node *min = findMin(root->right, x_dim, agents);	//find minimum of right node
		
			root->id = min->id;					//replace current node with minimum node
			root->index = min->index;

			root->right = deleteNodeRec(root->right, min->id, min->index, depth+1, agents);	//delete the old minimum
		}
		else if(root->left != NULL)		//same as above for left subtree
		{
			node *min = findMin(root->left, x_dim, agents);
			
			root->id = min->id;
			root->index = min->index;
			root->right = deleteNodeRec(root->left, min->id, min->index, depth+1, agents);
			root->left = NULL;
		}
		else{			//if node has no children, then delete it
			delete root;
			return NULL;
		}
		
		return root;
	}

	//if current node doesnt contain the required point
	if(x_dim == 0){
		if(agents[index].xPos() < agents[root->index].xPos())			
			root->left = deleteNodeRec(root->left, id, index, depth+1, agents);
		else root->right = deleteNodeRec(root->right, id, index, depth+1, agents);
	}
	//if y dimension
	else {
		if(agents[index].yPos() < agents[root->index].yPos())
			root->left = deleteNodeRec(root->left, id, index, depth+1, agents);
		else root->right = deleteNodeRec(root->right, id, index, depth+1, agents);
	}

	return root;
}

//----DELETE NODE----//
node *deleteNode(node *root, int id, int index, std::vector<Person> agents)
{
	return deleteNodeRec(root, id, index, 0, agents);
}

//----SEARCH FOR INDEX----//
bool searchRec(node* root, int index, int depth, std::vector<Person> agents)
{
//	std::cout << "agents[" << root->index << "] = (" << agents[root->index].xPos() << ", " << agents[root->index].yPos() << ")\n";
	
	if(root == NULL)
		return false;
	if(index == root->index)
		return true;
	if(index > (int)agents.size())
		return false;

	int x_dim = depth % 2;

	if(x_dim == 0){
		if(agents[index].xPos() < agents[root->index].xPos()){
//			std::cout << "MOVE LEFT\n";
			return searchRec(root->left, index, depth+1, agents);
		}
//		std::cout << "MOVE RIGHT\n";
		return searchRec(root->right, index, depth+1, agents);
	}
	else{
		if(agents[index].yPos() < agents[root->index].yPos()){
//			std::cout << "MOVE LEFT\n";
			return searchRec(root->left, index, depth+1, agents);
		}
//		std::cout << "MOVE RIGHT\n";
		return searchRec(root->right, index, depth+1, agents);
	}
}

bool search(node* root, int index, std::vector<Person> agents)
{
	return searchRec(root, index, 0, agents);	
}

//----OUTPUT INDEX GIVEN ID (RECURSIVE)----//
int indexFromIDRec(node* root, int id, double agent_x, double agent_y, int depth, std::vector<Person> agents)
{
	if(root == NULL)
		return 0;
	if(id == root->id)
		return root->index;

	int x_dim = depth % 2;

	if(x_dim == 0){
		if(agent_x < agents[root->index].xPos()){
			return indexFromIDRec(root->left, id, agent_x, agent_y, depth+1, agents);
		}
		return indexFromIDRec(root->right, id, agent_x, agent_y, depth+1, agents);
	}
	else{
		if(agent_y < agents[root->index].yPos()){
			return indexFromIDRec(root->left, id, agent_x, agent_y, depth+1, agents);
		}
		return indexFromIDRec(root->right, id, agent_x, agent_y, depth+1, agents);
	}
}

//----OUTPUT INDEX GIVEN ID----//
int indexFromID(node* root, int id, double agent_x, double agent_y, std::vector<Person> agents)
{
	return indexFromIDRec(root, id, agent_x, agent_y, 0, agents);	
}

void DeleteTreeRec(node *leaf)
{
	if(leaf != NULL)
	{
		DeleteTreeRec(leaf->left);
		DeleteTreeRec(leaf->right);
		delete leaf;
	}
}
//----DELETE FULL TREE----//
void DeleteTree(node *root)
{
	DeleteTreeRec(root->left);
	DeleteTreeRec(root->right);
}
