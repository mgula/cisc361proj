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
 * Struct or class?
 * what should the output actually be?
 * numeric constants?
 */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cmath>
#include "Node.h"

using namespace std;

#define MAX_NUMBER_INPUTS 50

int realTime = 0;
int inputNumber = 0;
bool simulating = true;
bool inputCompleted = false;
int currentInputTime;
int numberOfInputs;

int memory = 0;
int devices = 0;
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
	system->listSize = 1;
	system->next = NULL;

	Node *submitQueue = new Node;
	submitQueue->head = true;
	submitQueue->listSize = 1;
	submitQueue->next = NULL;

	Node *holdQueue1 = new Node;
	holdQueue1->head = true;
	holdQueue1->listSize = 1;
	holdQueue1->next = NULL;

	Node *holdQueue2 = new Node;
	holdQueue2->head = true;
	holdQueue2->listSize = 1;
	holdQueue2->next = NULL;

	Node *readyQueue = new Node;
	readyQueue->head = true;
	readyQueue->listSize = 1;
	readyQueue->next = NULL;

	Node *waitQueue = new Node;
	waitQueue->head = true;
	waitQueue->listSize = 1;
	waitQueue->next = NULL;

	Node *completeQueue = new Node;
	completeQueue->head = true;
	completeQueue->listSize = 1;
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
			strcpy(parsed, current.c_str()); //convert string to char array
			char * temp;
			temp = strtok(parsed, " "); //split string with space as delimiter
			int i = 0;
			currentInputTime = 0;
			while (i != 2) {
				if (i == 1) { //we know the second char cluster in temp must be the time
					for (int j = 0; j < strlen(temp); j++) { //translate char to int
						currentInputTime += pow(10, strlen(temp) - j - 1) * (temp[j] - '0');
					}
				}
				temp = strtok(NULL, " ");
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
		cout << realTime << " " << currentInputTime << endl;

		/*Perform queue maintenance*/






		realTime++;
	}

	cout << memory << " " << devices << " " << quantum << endl;
	testPrint(submitQueue);
	return 1;
}

void readCommand(string input, Node *sys, Node *submit) {
	//Convert the string to a char array and split (space as delimiter)
	char parsed[input.length()];
	strcpy(parsed, input.c_str());
	char *str;
	str = strtok(parsed, " ");

	//Pass the current input to appropriate method
	if (input[0] == 'C') {
		//System configuration
		configureSystem(str);
	} else if (input[0] == 'A') {
		//Arrival of a job
		createJob(str, sys, submit);
	} else if (input[0] == 'Q') {
		//A request for devices
	} else if (input[0] == 'L') {
		//A release of devices
	} else {
		//Unrecognized input
	}
}

void statusDisplay(string input, Node *sys, Node *submit, Node *hold1, Node *hold2, Node *ready, Node *wait, Node *complete) {
	if (input == "D 9999 " || input == "D 9999") {
		simulating = false;
		//Print system turnaround times
	}
	cout << "System history: " << endl;
	traverseAndPrint(sys);
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
		} else if (str[0] == 'S') {
			devices = extractFromString(str);
		} else if (str[0] == 'Q') {
			quantum = extractFromString(str);
		} else {
			//Unrecognized input
		}
		str = strtok (NULL, " ");
	}
}

void createJob(char *str, Node *sys, Node *submit) {
	Node *job = new Node;
	job->head = false;
	job->arrivalTime = currentInputTime;
	job->next = NULL;

	while (str != NULL) {
		if (str[0] == 'J') {
			job->jobNumber = extractFromString(str);
		} else if (str[0] == 'M') {
			job->jobMemory = extractFromString(str);
		} else if (str[0] == 'S') {
			job->jobDevices = extractFromString(str);
		} else if (str[0] == 'R') {
			job->runTime = extractFromString(str);
		} else if (str[0] == 'P') {
			job->jobPriority = extractFromString(str);
		} else {
			//Unrecognized input
		}
		str = strtok (NULL, " ");
	}
	add(submit, job);

	Node *copy = new Node;
	//try duplicating job
	addToEnd(sys, copy);
}

int extractFromString(char *str) {
	int x = 0;
	int i = strlen(str) - 1;
	int j = 0;
	while (str[i] != '=') {
		x += pow(10, j) * (str[i] - '0');
		j++;
		i--;
	}
	return x;
}
