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

void printSystem(Node *list) {
	Node *temp = list;
	if (temp->next == NULL) {
		cout << "This Queue is empty." << endl;
	} else {
		int i = 1;
		while (temp != NULL) {
			if (temp->head == false) {
				cout << "Job number: " << temp->jobNumber << " | Status: " << temp->status << " ";
				if (temp->status == "Completed") {
					cout << "| Turnaround Time: " << temp->turnaroundTime;
				} else {
					cout << "| Time Remaining: " << temp->remainingTime;
				}
				cout << endl;
				i++;
			}
			temp = temp->next;
		}
	}
}

/*Traverse a list and print information about each node*/
void traverseAndPrint(Node *list) {
	Node *temp = list;
	if (temp->next == NULL) {
		cout << "This Queue is empty." << endl;
	} else {
		int i = 1;
		while (temp != NULL) {
			if (temp->head == false) {
				cout << "Place in queue: " << i << " | Job number: " << temp->jobNumber << " " << endl;
				i++;
			}
			temp = temp->next;
		}
	}
}

void insert(Node *first, Node *second, Node *newNode) {
	first->next = newNode;
	newNode->next = second;
}

/*Add node to front of list*/
void addToFront(Node *head, Node *addition) {
	Node *temp = new Node;
	temp->next = head->next;
	head->next = addition;
	addition->next = temp->next;
}

/*Add node to end of list*/
void addToEnd(Node *head, Node *addition) {
	if (head->next == NULL) {
		head->next = addition;
		addition->next = NULL;
	} else {
		Node *temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = addition;
		addition->next = NULL;
	}

}

Node *remove(Node *head, int selectedJob){
	Node *temp = head;
	Node *tempPrev = NULL; //keep track of previous node

	while(temp != NULL) {
		if (temp->jobNumber == selectedJob) {//if jumber number matches what we want
			tempPrev->next = temp->next; //removing selected node from queue
			temp->next = NULL;
			return temp; //return removed node
		}
		tempPrev = temp;
		temp = temp->next;
	}

	return NULL; //Can't find node
}
