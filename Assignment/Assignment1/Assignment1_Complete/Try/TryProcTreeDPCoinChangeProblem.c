//
//  ProcTreeDPCoinChangeProblem.c
//  CSE_CCodes
//
//  Created by Natalie Agus on 4/1/19.
//  Copyright © 2019 Natalie Agus. All rights reserved.
//

#include "ProcTreeDPCoinChangeProblem.h"

/*
 Entry function. DO NOT CHANGE THIS FUNCTION
 */
void getCoinChangeCombination_test(int* coinSupplyValue, int value, int coinTypes, methodType method){
    
    //allocate memory for (value+1) * coinTypes ProcessTreeNode struct (each represents a cell in the DP table)
    //create (value+1) * coinTypes ProcessTreeNode structs (each represents a cell in the DP table)
    int totalNodes = (value+1) * coinTypes;
    ProcessTreeNode nodesDP[totalNodes];
    
    int counter = 0;
    for(int i = 0; i<(coinTypes); i++){
        for (int j = 0; j<(value+1); j++){
            initProcessTreeNode(&nodesDP[counter], j, i+1, counter, totalNodes, 0);
            counter ++;
        }
    }
    
    //assert the total number of nodes created is the same as totalNodes
    assert(totalNodes == counter);
    
    //link children and parents to each node
    linkRelationshipProcessTreeNode(nodesDP, coinSupplyValue, value, coinTypes);
    
    
    switch (method){
        case (USESEQUENTIAL):
            runFunctionSequentialMethod_Create(nodesDP);
            break;
        case (USETHREAD):
            runFunctionThreadMethod_Create(nodesDP);
            break;
        case (USEPROCESS):
        {
            int forkReturnVal = fork();
            if (forkReturnVal == 0)
                runFunctionProcessMethod_Create(nodesDP);
            else{
                wait(NULL);
            }
            break;
        }
    }
    
    
    printTreeForDebugging(nodesDP, counter);
}

void initProcessTreeNode(ProcessTreeNode* P, int value, int coinTypes, int number, int totalNodes, int initVal){
    P->numberOfParents = 0;
    P->numberOfChildren = 0;
    P->nodeNumber = number;
    sprintf(P->name, "S%dV%d", coinTypes, value);
    P->totalNumberOfNodes = totalNodes;
    P->cellValue = initVal;
    P->processStatus = UNDEFINED;
}

void printTreeForDebugging(ProcessTreeNode* nodesDP, int items){
    //try printing their names and children, as well as parents,
    //to see if they have been created correctly
    printf("Total number of nodes is %d \n", items);
    for (int i = 0; i<items; i++){
        printf("\nNode number %d with name: %s ", nodesDP[i].nodeNumber, nodesDP[i].name);
        printf("has %d children. The children are: ", nodesDP[i].numberOfChildren);
        for (int j = 0; j < nodesDP[i].numberOfChildren; j++){
            printf("%s, ", nodesDP[i].children[j]->name);
        }
        printf("and has %d parents. The parents are: ", nodesDP[i].numberOfParents);
        for (int j = 0; j < nodesDP[i].numberOfParents; j++){
            printf("%s, ", nodesDP[i].parents[j]->name);
        }
        printf("\nThe value of this node is: %d ", nodesDP[i].cellValue);
        printf("\n\n");
        
    }
}


/**
 Part 2
 @param int* coinSupplyValue: gives the denomination of coins (unlimited supply for each denomination)
 @param int value: that you want to make up using the coins
 @param int cointTypes: the total size of array coinSupplyValue (how many different coin types are there)
 **/

void linkRelationshipProcessTreeNode(ProcessTreeNode* nodesDP, int* coinSupplyValue, int value, int coinTypes)
{
    /** Your code here **/

	int count = 0;

	for (int i=0; i < coinTypes; i++) {		// row

		for (int j=0; j < value+1; j++) {	// col

			// fill in the attributes for ProcessTreeNode
			nodesDP[count].totalNumberOfNodes = (value + 1)*coinTypes;
			nodesDP[count].nodeNumber = count;

			nodesDP[count].inputValue = value;
			nodesDP[count].v = j;
			nodesDP[count].s = coinSupplyValue[i];

			// convert int to char using sprintf
			char nameForS[10];
			sprintf(nameForS, "%d", coinSupplyValue[i]);

			char nameForV[10];
			sprintf(nameForV, "%d", j);


			strcpy(nodesDP[count].name, "S");		// strcpy first before strcat cause it is un-initialised
			strcat(nodesDP[count].name, nameForS);
			strcat(nodesDP[count].name, "V");
			strcat(nodesDP[count].name, nameForV);

			
			// root -> S1V0
			if (count == 0) {
				// printf("root, count is %d\n", count);
				nodesDP[count].is_root = true;
				nodesDP[count].is_leaf = false;
			}

			// leaf -> the given node
			else if (count == ((value+1)*coinTypes - 1)) {
				// printf("leaf, count is %d\n", count );
				nodesDP[count].is_root = false;
				nodesDP[count].is_leaf = true;
			}

			// other nodes
			else if (count > 0 && count < ((value + 1)*coinTypes - 1)) {
				// printf("other, count is %d\n", count);
				nodesDP[count].is_root = false;
				nodesDP[count].is_leaf = false;
			}

			// all nodes starting from S=2 row has a parent node directly above it
			// EG. S2V4 ---> S1V4
			if (i >= 1) {			// i=0 --> first row
				// .children & .parents are arrays

				// set child --> content of nodesDP[count]
				// EG for V=4, type=3 --> node 0 has a child at node 5 --> difference in index is (value+1)
				int indexParent1 = count - (value + 1);
				nodesDP[indexParent1].children[nodesDP[indexParent1].numberOfChildren] = &nodesDP[count];
				nodesDP[indexParent1].numberOfChildren++;			// increment no. of children for that node
			

				// nodesDP[count].numberOfParents -> 0, 1, 2 (but won't be 2 when trying to add parent since there are only 2 ifs)
				nodesDP[count].parents[nodesDP[count].numberOfParents] = &nodesDP[indexParent1];
				nodesDP[count].numberOfParents++;
				
			}

			// if condition because a node can have up to 2 parents
			// assign parent to the left; but for nth row, its nth position to the left
			// coinSupplyValue[i] gives S[m]
			if ((count % (value + 1)) - coinSupplyValue[i] >= 0) {

				// set child
				// EG. for V=4, type=3: S=2; set node 7 to be child of node 5
				int indexParent2 = count - coinSupplyValue[i];
				nodesDP[indexParent2].children[nodesDP[indexParent2].numberOfChildren] = &nodesDP[count];
				nodesDP[indexParent2].numberOfChildren++;

				// set parent
				//EG. set node 5 to be parent of node 7 (current node)
				nodesDP[count].parents[nodesDP[count].numberOfParents] = &nodesDP[indexParent2];
				nodesDP[count].numberOfParents++;

			}
			// initialise cellValue -- root will be -1
			nodesDP[count].cellValue = -1 - count;		// initialise cellValue
			count++;	// increment index
		}
	}

}

/**
 Part 3
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionSequentialMethod_Create(ProcessTreeNode* root){
   /** Your code here **/

	// bottom-up approach
	for (int i = 1; i < root->totalNumberOfNodes; i++) {
		if (root->is_root) {
			// set root's cell value to be 1
			root->cellValue = 1;
			//printf("root node is set to 1, node= %s", root->name);
		}
		else {
			if (root->numberOfParents == 1) {
				// child = parent1
				root->cellValue = root->parents[0]->cellValue;
			}
			else if (root->numberOfParents == 2) {
				// child = parent1 + parent2
				root->cellValue = root->parents[0]->cellValue + root->parents[1]->cellValue;
			}
		}
		root++;
	}
}


/**
 Part 4
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionThreadMethod_Create(ProcessTreeNode* root){
    /** Your code here **/
	
	int numberOfThreads = root->totalNumberOfNodes;

	// thread identifier
	pthread_t threads[numberOfThreads];

	// first thread
	pthread_create(&threads[0], NULL, fillNodeThread, (void*) root);
	root++;

	// rest
	for (int i = 1; i < numberOfThreads; i++) {
		pthread_join(threads[i - 1], NULL);				// collect results from prev
		pthread_create(&threads[i], NULL, fillNodeThread, (void*)root);
		root++;
	}

	// join last thread
	pthread_join(threads[numberOfThreads - 1], NULL);
}

/**
 Part 5
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionProcessMethod_Create(ProcessTreeNode* root){
    /** Your code here **/


	// ftok to generate unique key 
	key_t key = ftok("shmfile", 65);

	// shmget returns an identifier in shmid
	// second arg = size of shared memory segment
	int shmid = shmget(key, (sizeof(int)) * root->totalNumberOfNodes, 0666 | IPC_CREAT);

	// shmat to attach to shared memory
	int* shm = shmat(shmid, NULL, 0);
	int* sm_server = shm;

	for (int i = 0; i < root->totalNumberOfNodes; i++) {
		*sm_server = root[i].cellValue;
		sm_server++;	// increment to next available spot
	}

	printf("Spawn processes\n");

	for (int i = 0; i < root->totalNumberOfNodes; i++) {
		pid_t pid = fork();

		if (i > 0) root++;
		if (pid < 0) {
			printf("Error while forking\n");
			exit(1);
		}
		else if (pid == 0) {
			// child process
			printf("Child Process %d. pid: %d. Node %s\n", i, (int)getpid(), root->name);
			shm += i;

			printf("Readings from segment: %d\n", *shm); // check our shared memory. should give us -1 to -totalnumberofnodes

			if (*shm == -1) {
				// root node
				*shm = 1;
				printf("Child (%d): pid %d. set root node to %d\n", i, (int)getpid(), *shm);
			}
			else if (*shm < 0) {
				while (*(shm - 1) < 0) {
					// do nothing
				}

				if (i< root->)
			}
		}
	}
	



}

/**YOU ARE FREE TO CREATE OTHER HELPER METHODS BELOW THIS LINE**/

void* fillNodeThread(void* node) {
	ProcessTreeNode* root = (ProcessTreeNode*) node;

	if (root->is_root) {
		// root node, so answer is 1
		root->cellValue = 1;
	}
	else if (root->numberOfParents == 1) {
		// child = parent1
		root->cellValue = root->parents[0]->cellValue;
	}
	else if (root->numberOfParents == 2) {
		// child = parent1 + parent2
		root->cellValue = root->parents[0]->cellValue + root->parents[1]->cellValue;
	}
	return NULL;
}