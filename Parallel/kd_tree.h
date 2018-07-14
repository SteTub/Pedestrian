//header guard
#ifndef __KD_H_INCLUDED__
#define __KD_H_INCLUDED__

//#include<iostream>
#include "Agent_par.h"

//forward declare Person Class
class Person;

typedef struct node{

	int id;			//identifier of each agent
	int index;		//position of agent in std::vector
	node *left, *right;
} node;

node* newNode(int agent_index, int agent_id);
node *insertRec(node *root, int agent_index, std::vector<Person> agents, int depth);
node* insert(node *root, int current_index, std::vector<Person> agents);
node *minNode(node *x, node *y, node *z, int x_dim, std::vector<Person> agents);
node *findMinRec(node *root, int x_dim, int depth, std::vector<Person> agents);
node *findMin(node* root, int x_dim, std::vector<Person> agents);
node* deleteNodeRec(node *root, int id, int index, int depth, std::vector<Person> agents);
node *deleteNode(node *root, int id, int index, std::vector<Person> agents);
bool searchRec(node* root, int index, int depth, std::vector<Person> agents);
bool search(node* root, int index, std::vector<Person> agents);
int indexFromIDRec(node* root, int id, double agent_x, double agent_y, int depth, std::vector<Person> agents);
int indexFromID(node* root, int id, double agent_x, double agent_y, std::vector<Person> agents);
void DeleteTreeRec(node *leaf);
void DeleteTree(node *root);

#endif
