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
	struct Node *next;
	int listSize;

	string status;
	int arrivalTime;
	int completionTime;

	int turnaroundTime;

	int jobNumber;
	int jobPriority;
	int runTime;
	int remainingTime;
	int jobMemory;
	int jobDevices;
	int jobDevicesRequested;
};

void printSystem(Node *x);
void traverseAndPrint(Node *x);
void insert(Node *x, Node *y, Node *z);
void addToFront(Node *x, Node *y);
void addToEnd(Node *x, Node *y);
Node *remove(Node *x, int i);

void testPrint(Node *x);

#endif /* NODE_H_ */
