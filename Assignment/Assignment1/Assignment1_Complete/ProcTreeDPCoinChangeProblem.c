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
}

/**
 Part 3
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionSequentialMethod_Create(ProcessTreeNode* root){
   /** Your code here **/
    
}


/**
 Part 4
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionThreadMethod_Create(ProcessTreeNode* root){
    /** Your code here **/
}

/**
 Part 5
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionProcessMethod_Create(ProcessTreeNode* root){
    /** Your code here **/
}

/**YOU ARE FREE TO CREATE OTHER HELPER METHODS BELOW THIS LINE**/
