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


void readCommand(string input, Node *submitQueue);
void statusDisplay(string input);
void configureSystem(char *str);
void createJob(char *str, Node *submitQueue);

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
			statusDisplay(current);
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
			readCommand(current, submitQueue);
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
	traverseAndPrint(submitQueue);
	return 1;
}

void readCommand(string input, Node *submitQueue) {
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
		createJob(str, submitQueue);
	} else if (input[0] == 'Q') {
		//A request for devices
	} else if (input[0] == 'L') {
		//A release of devices
	} else {
		//Unrecognized input
	}
}

void statusDisplay(string input) {
	if (input == "D 9999 " || input == "D 9999") {
		simulating = false;
	}
	char parsed[input.length()];
	strcpy(parsed, input.c_str());
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

void createJob(char *str, Node *submitQueue) {
	Node *job = new Node;
	job->head = false;
	job->arrivalTime = currentInputTime;

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
	add(submitQueue, job);
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
