/*
 * Node.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: marcusgula
 */

#include <iostream>
#include <stdlib.h>
#include "Node.h"

using namespace std;

void traverseAndPrint(Node *list) {
	Node *temp = list;
	while (temp != NULL) {
		if (temp->head == false) {
			cout << temp->arrivalTime << " " << temp->jobNumber << " " << temp->jobMemory << " " << temp->jobDevices << " " << temp->runTime << " " << temp->jobPriority << endl;
		}
		temp = temp->next;
	}
	cout << endl;
}

void insert(Node *first, Node *second, Node *newNode) {
	first->next = newNode;
	newNode->next = second;
}

void add(Node *list, Node *addition) {
	Node *temp = new Node;
	temp->next = list->next;
	list->next = addition;
	addition->next = temp->next;
}




