/*
 * main.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: marcusgula
 *
 * This program assumes there is ALWAYS a space after each integer. Problems may arise if
 * the text file contains a line that doesn't end in a space.
 *
 *
 * Questions for TA:
 * what should the output actually be?
 * numeric constants?
 */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include "Node.h"

using namespace std;


#define MAX_NUMBER_INPUTS 50

/*Possible statuses of nodes*/
#define REJECTED "Rejected"
#define SUBMIT_QUEUE "Submit Queue"
#define HOLD_QUEUE_1 "Hold Queue 1"
#define HOLD_QUEUE_2 "Hold Queue 2"
#define READY_QUEUE "Ready Queue"
#define RUNNING "Running on the CPU"
#define WAIT_QUEUE "Wait Queue"
#define COMPLETE_QUEUE "Complete Queue"

int realTime = 0;
int inputNumber = 0;
bool simulating = true;
bool inputCompleted = false;
int currentInputTime;
int numberOfInputs;

int memory = 0;
int currentMemory = 0;
int devices = 0;
int currentDevices = 0;
int quantum = 0;


void readCommand(string input, Node *sys, Node *submit);
void statusDisplay(string input, Node *sys, Node *submit, Node *hold1, Node *hold2, Node *ready, Node *wait, Node *complete);
void configureSystem(char *str);
void createJob(char *str, Node *sys, Node *submit);

int extractFromString(char *str);

int main () {

	/*Read input from the target text file and place in a queue of strings*/
	string line;
	string queue[MAX_NUMBER_INPUTS];
	ifstream myfile ("/Users/marcusgula/Desktop/testInput");
	if (myfile.is_open()) {
		int i = 0;
		while (getline(myfile, line)) {
			queue[i] = line;
			i++;
		}
		numberOfInputs = i;
		myfile.close();
	} else {
		cout << "Unable to open selected file.";
		return 0;
	}

	/*Create necessary queues*/
	Node *system = new Node;
	system->head = true;
	system->next = NULL;

	Node *submitQueue = new Node;
	submitQueue->head = true;
	submitQueue->next = NULL;

	Node *holdQueue1 = new Node;
	holdQueue1->head = true;
	holdQueue1->next = NULL;

	Node *holdQueue2 = new Node;
	holdQueue2->head = true;
	holdQueue2->next = NULL;

	Node *readyQueue = new Node;
	readyQueue->head = true;
	readyQueue->next = NULL;

	Node *runningQueue = new Node;
	runningQueue->head = true;
	runningQueue->next = NULL;

	Node *waitQueue = new Node;
	waitQueue->head = true;
	waitQueue->next = NULL;

	Node *completeQueue = new Node;
	completeQueue->head = true;
	completeQueue->next = NULL;

	/*Begin simulation of system*/
	while (simulating) {

		string current = queue[inputNumber];

		/*Update time of the current input, unless current input is a status display*/
		if (current[0] == 'D') {
			statusDisplay(current, system, submitQueue, holdQueue1, holdQueue2, readyQueue, waitQueue, completeQueue);
			inputCompleted = true;
		} else if (current[0] == 'C' || current[0] == 'A' || current[0] == 'Q' || current[0] == 'L') {
			char parsed[current.length()];
			std::strcpy(parsed, current.c_str()); //Convert string to char array - type out std:: just to be safe
			char * temp;
			temp = std::strtok(parsed, " "); //Split string with space as delimiter
			int i = 0;
			currentInputTime = 0;
			while (i != 2) {
				if (i == 1) { //We know the second char cluster in temp must be the time
					for (int j = 0; j < std::strlen(temp); j++) { //translate char to int
						currentInputTime += pow(10, std::strlen(temp) - j - 1) * (temp[j] - '0');
					}
				}
				temp = std::strtok(NULL, " ");
				i++;
			}
		}

		/*Make sure to only process the current input once*/
		if (!inputCompleted) {
			readCommand(current, system, submitQueue);
			inputCompleted = true;
		}

		/*Move on to next command if current command has been processed*/
		if (realTime >= currentInputTime && inputCompleted) {
			inputNumber++;
			inputCompleted = false;
		}

		/*Perform queue maintenance*/
		if (submitQueue->next != NULL) {
			Node *temp = submitQueue;
			while (temp != NULL) {
				if (temp->head == false) {
					if (temp->jobMemory > memory || temp->jobDevices > devices) {
						/*Reject this job - can never satisfy*/
						remove(submitQueue, temp->jobNumber);

						/*Update system history*/
						Node *temp1 = system;
						while (temp1 != NULL) {
							if(temp1->jobNumber == temp->jobNumber) {
								temp1->status = REJECTED;
							}
							temp1 = temp1->next;
						}
					}
					/*Transfer from submit queue to hold queues*/
					Node *transfer = remove(submitQueue, temp->jobNumber);
					if (transfer != NULL) {
						if (transfer->jobPriority == 1) {
							addToFront(holdQueue1, transfer);

							/*Update system history*/
							Node *temp1 = system;
							while (temp1 != NULL) {
								if(temp1->jobNumber == temp->jobNumber) {
									temp1->status = HOLD_QUEUE_1;
								}
								temp1 = temp1->next;
							}
						} else if (transfer->jobPriority == 2) {
							addToFront(holdQueue2, transfer);

							/*Update system history*/
							Node *temp1 = system;
							while (temp1 != NULL) {
								if(temp1->jobNumber == temp->jobNumber) {
									temp1->status = HOLD_QUEUE_2;
								}
								temp1 = temp1->next;
							}
						}
					}
				}
				temp = temp->next;
			}
		}

		//waitqueue

		//hold queue 1

		//hold queue 2

		//ready

		//running
			//hold queues (bcuz memory was released)

		//complete




		/*Increment real time*/
		realTime++;
	}

	cout << memory << " " << devices << " " << quantum << endl << endl;
	testPrint(system);
	return 1;
}

void readCommand(string input, Node *sys, Node *submit) {
	/*Convert the string to a char array and split (space as delimiter)*/
	char parsed[input.length()];
	std::strcpy(parsed, input.c_str());
	char *str;
	str = std::strtok(parsed, " ");

	/*Pass the current input to appropriate method*/
	if (input[0] == 'C') {
		/*System configuration*/
		configureSystem(str);
	} else if (input[0] == 'A') {
		/*Arrival of a job*/
		createJob(str, sys, submit);
	} else if (input[0] == 'Q') {
		/*A request for devices*/
	} else if (input[0] == 'L') {
		/*A release of devices*/
	} else {
		/*Unrecognized input*/
	}
}

void statusDisplay(string input, Node *sys, Node *submit, Node *hold1, Node *hold2, Node *ready, Node *wait, Node *complete) {
	if (input == "D 9999 " || input == "D 9999") {
		simulating = false;
		//Print system turnaround times
	}
	cout << "System history: " << endl;
	printSystem(sys);
	cout << endl << "Submit Queue contents: " << endl;
	traverseAndPrint(submit);
	cout << endl << "Hold Queue 1 contents: " << endl;
	traverseAndPrint(hold1);
	cout << endl << "Hold Queue 2 contents: " << endl;
	traverseAndPrint(hold2);
	cout << endl << "Ready Queue contents: " << endl;
	traverseAndPrint(ready);
	cout << endl << "Wait Queue contents: " << endl;
	traverseAndPrint(wait);
	cout << endl << "Complete Queue contents: " << endl;
	traverseAndPrint(complete);
	cout << endl;
}

void configureSystem(char *str) {
	while (str != NULL) {
		if (str[0] == 'M') {
			memory = extractFromString(str);
			currentMemory = memory;
		} else if (str[0] == 'S') {
			devices = extractFromString(str);
			currentDevices = devices;
		} else if (str[0] == 'Q') {
			quantum = extractFromString(str);
		} else {
			/*Unrecognized input*/
		}
		str = std::strtok(NULL, " ");
	}
}

void createJob(char *str, Node *sys, Node *submit) {
	Node *job = new Node;
	Node *copy = new Node; //Create a duplicate for the system history
	job->head = false;
	copy->head = false;
	job->status = SUBMIT_QUEUE;
	copy->status = SUBMIT_QUEUE;
	job->arrivalTime = currentInputTime;
	copy->arrivalTime = currentInputTime;
	job->next = NULL;
	copy->next = NULL;

	while (str != NULL) {
		if (str[0] == 'J') {
			job->jobNumber = extractFromString(str);
			copy->jobNumber = extractFromString(str);
		} else if (str[0] == 'M') {
			job->jobMemory = extractFromString(str);
			copy->jobMemory = extractFromString(str);
		} else if (str[0] == 'S') {
			job->jobDevices = extractFromString(str);
			copy->jobDevices = extractFromString(str);
		} else if (str[0] == 'R') {
			job->runTime = extractFromString(str);
			copy->runTime = extractFromString(str);
		} else if (str[0] == 'P') {
			job->jobPriority = extractFromString(str);
			copy->jobPriority = extractFromString(str);
		} else {
			/*Unrecognized input*/
		}
		str = std::strtok (NULL, " ");
	}

	addToEnd(submit, job);
	addToEnd(sys, copy);
}

int extractFromString(char *str) {
	int x = 0;
	int i = std::strlen(str) - 1;
	int j = 0;
	while (str[i] != '=') {
		x += pow(10, j) * (str[i] - '0');
		j++;
		i--;
	}
	return x;
}
