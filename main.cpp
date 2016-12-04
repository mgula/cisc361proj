/*
 * main.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: marcusgula
 *
 * This program assumes there is ALWAYS a space after each integer. Problems may arise if
 * the text file contains a line that doesn't end in a space.\
 *
 *
 * Questions for TA:
 * Can we assume the times of the commands will be from least to greatest?
 * "under no circumstance should you pre process the file"
 * new struct for processes?
 * what should the output actually be?
 * final report?
 * numeric constants?
 * two jobs arriving at same time?
 */

#include <iostream>
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

int memory;
int devices;
int quantum;

void readCommand(string input);
void statusDisplay(string input);
void configureSystem(string input);

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
			readCommand(current);
			inputCompleted = true;
		}
		/*Move on to next command if current command has been processed*/
		if (realTime >= currentInputTime && inputCompleted) {
			inputNumber++;
			inputCompleted = false;
		}

		cout << realTime << " " << currentInputTime << endl;







		realTime++;
	}

	cout << memory << " " << devices << " " << quantum << endl;
	cout << "end";
	return 1;
}

void readCommand(string input) {
	//Pass the current input to appropriate method
	if (input[0] == 'C') {
		//System configuration
		configureSystem(input);
	} else if (input[0] == 'A') {
		//Arrival of a job
	} else if (input[0] == 'Q') {
		//A request for devices
	} else if (input[0] == 'L') {
		//A release of devices
	} else {
		//Unrecognized input
	}
}

void configureSystem(string input) {
	char parsed[input.length()];
	strcpy(parsed, input.c_str());
	char * temp;
	temp = strtok(parsed, " ");
	while (temp != NULL) {
		if (temp[0] == 'M') {
			memory = 0;
			int i = strlen(temp) - 1;
			int j = 0;
			while (temp[i] != '=') {
				memory += pow(10, j) * (temp[i] - '0');
				j++;
				i--;
			}
		} else if (temp[0] == 'S') {
			devices = 0;
			int i = strlen(temp) - 1;
			int j = 0;
			while (temp[i] != '=') {
				devices += pow(10, j) * (temp[i] - '0');
				j++;
				i--;
			}
		} else if (temp[0] == 'Q') {
			quantum = 0;
			int i = strlen(temp) - 1;
			int j = 0;
			while (temp[i] != '=') {
				quantum += pow(10, j) * (temp[i] - '0');
				j++;
				i--;
			}
		}
		temp = strtok (NULL, " ");
	}
}

void statusDisplay(string input) {
	if (input == "D 9999 " || input == "D 9999") {
		simulating = false;

	}
	char parsed[input.length()];
	strcpy(parsed, input.c_str());
}
