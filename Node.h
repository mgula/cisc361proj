/*
 * Node.h
 *
 *  Created on: Dec 3, 2016
 *      Author: marcusgula
 */

#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cctype>

using namespace std;

struct Node {
	bool head;
	int arrivalTime;
	int completionTime;
	int jobNumber;
	int jobPriority;
	int runTime;
	int jobMemory;
	int jobDevices;
	struct Node *next;
};

void traverseAndPrint(Node *x);
void insert(Node *x, Node *y, Node *z);
void add(Node *x, Node *y);

#endif /* NODE_H_ */
