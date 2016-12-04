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

void traverse(Node *list) {
	Node *temp = list;
	while (temp != NULL) {
		cout << temp->data << endl;
		temp = temp->next;
	}
	cout << endl;
}

void insert(Node *first, Node *second, Node *newNode) {
	first->next = newNode;
	newNode->next = second;
}




