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
			//System status display
			statusDisplay(current);
			inputNumber++;
		} else if (current[0] == 'C' || current[0] == 'A' || current[0] == 'Q' || current[0] == 'L') {
			char temp[4] = {'a', 'a', 'a', 'a'}; //assume time of first input will be <9999
			int i = 2; //start at character 3 of the string
			while (current[i] != ' ') {
				temp[i - 2] = current[i];
				i++;
			}
			int valid = i - 2;
			currentInputTime = 0;
			i = 0;
			while (temp[i] != 'a') {
				currentInputTime += (pow(10, valid - i - 1)) * (temp[i] - '0');
				i++;
			}

			/*
			char parsed[current.length()];
			strcpy(parsed, current.c_str());
			char * temp;
			temp = strtok(parsed, " ");
			currentInputTime = 0;
			int i = 0;
			while (temp != NULL) {
				if (i == 1) {
					//this doesnt work after temp is more than 1 character long
					currentInputTime = *temp - '0';
				}
				temp = strtok (NULL, " ,.-");
				i++;
			}*/
		}

		if (!inputCompleted) {
			readCommand(current);
			inputCompleted = true;
		}

		if (realTime >= currentInputTime && inputCompleted) {
			inputNumber++;
			inputCompleted = false;
		}

		cout << realTime << " " << currentInputTime << endl;



		realTime++;
	}

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
	bool space;
	for (int i = 0; i < input.length(); i++) {
		if (i == 0 || i == 1) {
			continue;
		} else {

		}
	}
}

void statusDisplay(string input) {
	if (input == "D 9999 ") {
		simulating = false;
	}
	char parsed[input.length()];
	strcpy(parsed, input.c_str());
}
