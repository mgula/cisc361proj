/*
 * Marcus Gula
 * Thomas Nelson
 * CISC 361
 * Project 1
 *
 * This program assumes there is ALWAYS a space after each integer. Problems may arise if
 * the text file contains a line that doesn't end in a space.
 *
 * only release what you request plz
 *
 * TODO: general bugginess with run (?) queue
 * TODO: system turnaround time and weighted turnaround times
 * TODO: wait queue maintenance?
 * TODO: report
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
#define LONG_TIME 9999

/*Possible statuses of nodes*/
#define REJECTED "Rejected"
#define SUBMIT_QUEUE "Submit Queue"
#define HOLD_QUEUE_1 "Hold Queue 1"
#define HOLD_QUEUE_2 "Hold Queue 2"
#define READY_QUEUE "Ready Queue"
#define RUNNING "Running on the CPU"
#define WAIT_QUEUE "Wait Queue"
#define COMPLETED "Completed"

int realTime = 0;
int inputNumber = 0;
bool simulating = true;
bool inputCompleted = false;
bool allInputRead = false;
int currentInputTime;
int numberOfInputs;

int memory = 0;
int currentMemory = 0;
int devices = 0;
int currentDevices = 0;
int quantum = 0;
int quantumSlice = 0;

void readCommand(string input, Node *sys, Node *submit, Node *run, Node *ready, Node *wait);
void statusDisplay(string input, Node *sys, Node *submit, Node *hold1, Node *hold2, Node *ready, Node *run, Node *wait, Node *complete);
void configureSystem(char *str);
void createJob(char *str, Node *sys, Node *submit);
void makeRequest(char *str, Node *sys, Node *run, Node *ready, Node *wait);
void release(char *str, Node *sys, Node *run, Node *wait, Node *ready);

void updateSystem(Node *sys, Node *update, string status);
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

		/*Safety check*/
		if (current[current.length() - 1] != ' ') {
			cout << "Line not ending with a space (line " << inputNumber + 1 << ") - command ignored." << endl;
			inputNumber++;
			if (inputNumber >= numberOfInputs) {
				allInputRead = true;
			}
			continue;
		}

		/*Update time of the current input, unless current input is a status display*/
		if (!allInputRead) {
			if (current[0] == 'D') {
				statusDisplay(current, system, submitQueue, holdQueue1, holdQueue2, readyQueue, runningQueue, waitQueue, completeQueue);
				if (inputNumber < numberOfInputs - 1) {
					inputNumber++;
					if (inputNumber >= numberOfInputs) {
						allInputRead = true;
					}
				}
				continue;
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
		}

		/*Process the current input */
		if (!allInputRead && realTime == currentInputTime) {
			readCommand(current, system, submitQueue, runningQueue, readyQueue, waitQueue);
			inputNumber++;
			if (inputNumber > numberOfInputs) {
				allInputRead = true;
			}
		}

		/*Perform submit queue maintenance*/
		if (submitQueue->next != NULL) {
			Node *temp = submitQueue;
			while (temp != NULL) {
				if (temp->head == false) {
					if (temp->jobMemory > memory || temp->maxJobDevices > devices) {
						/*Reject this job - can never satisfy*/
						remove(submitQueue, temp->jobNumber);

						/*Update system status*/
						updateSystem(system, temp, REJECTED);
					}
					/*Transfer from submit queue to hold queues*/
					Node *transfer = remove(submitQueue, temp->jobNumber);
					if (transfer != NULL) {
						if (transfer->jobPriority == 1) {
							addToEnd(holdQueue1, transfer);

							/*Update system status*/
							updateSystem(system, transfer, HOLD_QUEUE_1);
						} else if (transfer->jobPriority == 2) {
							addToEnd(holdQueue2, transfer);

							/*Update system status*/
							updateSystem(system, transfer, HOLD_QUEUE_2);
						}
					}
				}
				temp = temp->next;
			}
		}

		/*Perform wait queue maintenance*/


		/*Perform hold queue 1 maintenance*/
		if (holdQueue1->next != NULL) {
			Node *temp = holdQueue1;
			int shortestJob = 0;
			int shortestJobTime = LONG_TIME;
			/*Get job number of shortest job*/
			while (temp != NULL) {
				if (temp->head == false) {
					if (temp->jobMemory <= currentMemory && temp->maxJobDevices <= currentDevices) {
						if (temp->runTime < shortestJobTime) {
							shortestJobTime = temp->runTime;
							shortestJob = temp->jobNumber;
						}
					}
				}
				temp = temp->next;
			}
			/*Remove job from hold queue 1 and add to ready queue*/
			if (shortestJob > 0){
				Node *transfer = remove(holdQueue1, shortestJob);
				addToEnd(readyQueue, transfer);
				currentMemory -= transfer->jobMemory;

				/*Update system status*/
				updateSystem(system, transfer, READY_QUEUE);
			}
		}

		/*Perform hold queue 2 maintenance*/
		if (holdQueue2->next != NULL) {
			Node *temp = holdQueue2;
			/*Get last node (first in)*/
			while (temp->next != NULL) {
				temp = temp->next;
			}
			if (temp->jobMemory <= currentMemory && temp->maxJobDevices <= currentDevices) {
				Node *transfer = remove(holdQueue2, temp->jobNumber);
				addToEnd(readyQueue, transfer);
				currentMemory = currentMemory - transfer->jobMemory;

				/*Update system status*/
				updateSystem(system, transfer, READY_QUEUE);
			}
		}

		/*Perform ready queue maintenance*/
		if (readyQueue->next != NULL) {
			if (runningQueue->next == NULL) {
				Node *transfer = remove(readyQueue, readyQueue->next->jobNumber);
				addToFront(runningQueue, transfer);
				quantumSlice = 0; //Reset quantum time

				/*Update system status*/
				updateSystem(system, transfer, RUNNING);
			}
		}


		/*Perform running queue maintenance*/
		if (runningQueue->next != NULL) {
			runningQueue->next->remainingTime--;
			quantumSlice++;

			/*Update system status*/
			Node *temp = system;
			while (temp->jobNumber != runningQueue->next->jobNumber) {
				temp = temp->next;
			}
			temp->remainingTime--;

			if (runningQueue->next->remainingTime == 0) {
				quantumSlice = 0;
				runningQueue->next->completionTime = realTime;
				runningQueue->next->turnaroundTime = realTime - runningQueue->next->arrivalTime;

				/*Update system status*/
				Node *temp = system;
				while (temp->jobNumber != runningQueue->next->jobNumber) {
					temp = temp->next;
				}
				temp->completionTime = realTime;
				temp->turnaroundTime = runningQueue->next->turnaroundTime;

				currentMemory += runningQueue->next->jobMemory;

				if (runningQueue->jobDevicesGranted) {
					currentDevices += runningQueue->devicesRequested;
				}

				Node *transfer = remove(runningQueue, runningQueue->next->jobNumber);
				addToEnd(completeQueue, transfer);

				/*Update system status*/
				updateSystem(system, transfer, COMPLETED);

				/*Check wait queue, then hold queue 1, then hold queue 2*/
				if (waitQueue->next != NULL) {
					Node *temp = waitQueue;
					while (temp != NULL) {
						if (temp->maxJobDevices <= currentDevices) {
							Node *transfer = remove(waitQueue, temp->jobNumber);
							addToFront(readyQueue, transfer);

							/*Update system status*/
							updateSystem(system, transfer, READY_QUEUE);
						}
						temp = temp->next;
					}
				}
				if (holdQueue1->next != NULL) {
					Node *temp = holdQueue1;
					int shortestJob = 0;
					int shortestJobTime = LONG_TIME;
					/*Get job number of shortest job*/
					while (temp != NULL) {
						if (temp->head == false) {
							if (temp->jobMemory <= currentMemory && temp->maxJobDevices <= currentDevices) {
								if (temp->runTime < shortestJobTime) {
									shortestJobTime = temp->runTime;
									shortestJob = temp->jobNumber;
								}
							}
						}
						temp = temp->next;
					}
					/*Remove job from hold queue 1 and add to ready queue*/
					if (shortestJob > 0){
						Node *transfer = remove(holdQueue1, shortestJob);
						addToEnd(readyQueue, transfer);
						currentMemory -= transfer->jobMemory;

						/*Update system status*/
						updateSystem(system, transfer, READY_QUEUE);
					}
				}
				if (holdQueue2->next != NULL) {
					Node *temp = holdQueue2;
					/*Get last node (first in)*/
					while (temp->next != NULL) {
						temp = temp->next;
					}
					if (temp->jobMemory <= currentMemory && temp->maxJobDevices <= currentDevices) {
						Node *transfer = remove(holdQueue2, temp->jobNumber);
						addToEnd(readyQueue, transfer);
						currentMemory = currentMemory - transfer->jobMemory;

						/*Update system status*/
						updateSystem(system, transfer, READY_QUEUE);
					}
				}
				/*Run next job on the CPU*/
				if (readyQueue->next != NULL) {
					Node *transfer = remove(readyQueue, readyQueue->next->jobNumber);
					addToEnd(runningQueue, transfer);

					/*Update system status*/
					updateSystem(system, transfer, RUNNING);
				}
			} else if (quantumSlice == quantum) {
				quantumSlice = 0;
				if (readyQueue->next != NULL) {
					Node *firstTransfer = remove(runningQueue, runningQueue->next->jobNumber);
					addToEnd(readyQueue, firstTransfer);
					Node *secondTransfer = remove(readyQueue, readyQueue->next->jobNumber);
					addToEnd(runningQueue, secondTransfer);

					/*Update system status*/
					updateSystem(system, firstTransfer, READY_QUEUE);
					updateSystem(system, secondTransfer, RUNNING);
				}
			}
		}

		/*Increment real time*/
		realTime++;

		/*End simulation only when all input completed and CPU finished*/
		if (runningQueue->next == NULL && allInputRead) {
			simulating = false;
		}
	}

	/*Final system display*/
	cout << "Final system status: " << endl;
	printSystem(system);
	cout << endl << "Submit Queue contents: " << endl;
	traverseAndPrint(submitQueue);
	cout << endl << "Hold Queue 1 contents: " << endl;
	traverseAndPrint(holdQueue1);
	cout << endl << "Hold Queue 2 contents: " << endl;
	traverseAndPrint(holdQueue2);
	cout << endl << "Ready Queue contents: " << endl;
	traverseAndPrint(readyQueue);
	cout << endl << "Running on the CPU: " << endl;
	traverseAndPrint(runningQueue);
	cout << endl << "Wait Queue contents: " << endl;
	traverseAndPrint(waitQueue);
	cout << endl << "Complete Queue contents: " << endl;
	traverseAndPrint(completeQueue);
	cout << endl << "System turnaround time: ";
	double i = 0;
	int j = 0;
	Node *temp = system;
	while (temp->next != NULL) {
		i += temp->turnaroundTime;
		j++;
		temp = temp->next;
	}
	i+= temp->turnaroundTime;
	i /= j;
	printf("%.2f", i);
	return 1;
}

void readCommand(string input, Node *sys, Node *submit, Node *run, Node *ready, Node *wait) {
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
		makeRequest(str, sys, run, ready, wait);
	} else if (input[0] == 'L') {
		/*A release of devices*/
		release(str, sys, run, wait, ready);
	} else {
		/*Unrecognized input*/
	}
}

void statusDisplay(string input, Node *sys, Node *submit, Node *hold1, Node *hold2, Node *ready, Node *run, Node *wait, Node *complete) {
	if (input == "D 9999 " || input == "D 9999") {
		allInputRead = true;
		return;
	}
	cout << "System status at time " << realTime + 1 << ": " << endl;
	printSystem(sys);
	cout << endl << "Submit Queue contents: " << endl;
	traverseAndPrint(submit);
	cout << endl << "Hold Queue 1 contents: " << endl;
	traverseAndPrint(hold1);
	cout << endl << "Hold Queue 2 contents: " << endl;
	traverseAndPrint(hold2);
	cout << endl << "Ready Queue contents: " << endl;
	traverseAndPrint(ready);
	cout << endl << "Running on the CPU: " << endl;
	cout << "Job Number: " << run->next->jobNumber << endl;
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
			job->maxJobDevices = extractFromString(str);
			copy->maxJobDevices = extractFromString(str);
		} else if (str[0] == 'R') {
			job->runTime = extractFromString(str);
			job->remainingTime = job->runTime;
			copy->runTime = extractFromString(str);
			copy->remainingTime = copy->runTime;
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

void makeRequest(char *str, Node *sys, Node *run, Node *ready, Node *wait) {
	quantumSlice = 0;
	int j = 0;
	int d = 0;
	while (str != NULL) {
		if (str[0] == 'J') {
			j = extractFromString(str);
		} else if (str[0] == 'D') {
			d = extractFromString(str);
		}
		str = std::strtok (NULL, " ");
	}
	run->next->devicesRequested = d;

	if (j != run->next->jobNumber) {
		return;
	}

	if (j == run->next->jobNumber && d <= currentDevices) {
		run->jobDevicesGranted = true;
		currentDevices -= d;
		Node *transfer = remove(run, run->next->jobNumber);
		addToEnd(ready, transfer);

		/*Update system status*/
		updateSystem(sys, transfer, READY_QUEUE);
	} else {
		run->jobDevicesGranted = false;
		Node *transfer = remove(run, run->next->jobNumber);
		addToEnd(wait, transfer);

		/*Update system status*/
		updateSystem(sys, transfer, WAIT_QUEUE);
	}
}

void release(char *str, Node *sys, Node *run, Node *wait, Node *ready) {
	quantumSlice = 0;
	int j = 0;
	int d = 0;
	while (str != NULL) {
		if (str[0] == 'J') {
			j = extractFromString(str);
		} else if (str[0] == 'D') {
			d = extractFromString(str);
		}
		str = std::strtok (NULL, " ");
	}

	if (j != run->next->jobNumber) {
		return;
	}

	if (run->next->jobDevicesGranted) {
		currentDevices += d;

		Node *temp = wait;
		while (temp->next != NULL) {
			if (temp->maxJobDevices <= currentDevices) {
				Node *transfer = remove(wait, temp->jobNumber);

				addToEnd(ready, transfer);
				/*Update system status*/
				updateSystem(sys, transfer, READY_QUEUE);
			}
			temp = temp->next;
		}
	}
}

void updateSystem(Node *sys, Node *update, string status) {
	Node *temp = sys;
	while (temp != NULL) {
		if(temp->jobNumber == update->jobNumber) {
			temp->status = status;
		}
		temp = temp->next;
	}
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
