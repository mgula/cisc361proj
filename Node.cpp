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
	if (temp->next == NULL) {
		cout << "This Queue is empty." << endl;
	} else {
		int i = 1;
		while (temp != NULL) {
			if (temp->head == false) {
				cout << "Place in queue: " << i << " Job number: " << temp->jobNumber << " " << endl;
				i++;
			}
			temp = temp->next;
		}
		cout << endl;
	}
}

void testPrint(Node *list) {
	Node *temp = list;
	if (temp->next == NULL) {
		cout << "This Queue is empty." << endl;
	} else {
		while (temp != NULL) {
			if (temp->head == false) {
				cout << temp->arrivalTime << " " << temp->jobNumber << " " << temp->jobMemory << " " << temp->jobDevices << " " << temp->runTime << " " << temp->jobPriority << endl;
			}
			temp = temp->next;
		}
		cout << endl;
	}
}

void insert(Node *first, Node *second, Node *newNode) {
	first->next = newNode;
	newNode->next = second;
}

/*Add node to front of list*/
void add(Node *head, Node *addition) {
	Node *temp = new Node;
	temp->next = head->next;
	head->next = addition;
	addition->next = temp->next;

	head->listSize++;
}

/*Add node to end of list*/
void addToEnd(Node *head, Node *addition) {
	if (head->next == NULL) {
		head->next = addition;
		addition->next = NULL;

		head->listSize++;
	} else {
		if (head->next->next == NULL) {
			head->next->next = addition;
			addition->next = NULL;

			head->listSize++;
		} else {
			/*PROBLEM: this messes up other lists*/
			Node *temp = new Node;
			temp->next = head->next;
			while (temp->next->next != NULL) {
				temp = temp->next;
			}
			temp->next = addition;
			addition->next = NULL;
		}
	}

}




