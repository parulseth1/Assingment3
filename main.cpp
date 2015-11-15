/* 
 * File:   main.cpp
 * Author: parul
 *
 * Created on November 2, 2015, 7:05 PM
 */

#include <cstdlib>
#include "Partition.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "BandB.h"
#include "Parser.h"
#include <string.h>
#include "Parallel.h"
#include "Drawing.h"
#include <pthread.h>

//#define FILENAME "/home/parul/NetBeansProjects/BandBPartition/cct4.txt"

#define PARENT (-1)
#define LEFT_CHILD 1


using namespace::std;

int main(int argc, const char * argv[]) {
    
    
    if (argc < 3) {
        cout << "Usage : ./bandbpartition -u <File name>" << endl;
        return 1;
    }
    int caseDirection;
    if (strcmp(argv[1], "-s") == 0) {
        //unidirectional case
        caseDirection = 1;
    } else if (strcmp(argv[1], "-p") == 0) {
        //bidirectional case
        caseDirection = 2;
    } else {
        cout << "wrong argument" << endl;
        return 1;
    }
    
    
    
    char* FILENAME = new char[strlen(argv[2]) + 1];
    strcpy(FILENAME, argv[2]);
    cout<<FILENAME<<endl;
    
    vector<block> Blocks;
    int numOfBlocks;
    Net* Nets = NULL;
    int numNets = 0;
    doParse(FILENAME, &Blocks, &Nets, &numNets);
    
//    cout<<"Parsing Complete"<<endl;
//    cout<<"number of nets: "<<numNets<<endl;
         numOfBlocks = Blocks.size();
//    for (int i = 0; i < numNets; i++){
//
//        cout<<i+1<<"::"<<Nets[i].getNumPins()<<endl;
//    }
    int i;
    int* order = new int[numOfBlocks];
    for (i = 0; i<numOfBlocks; i++){
        order[i] = i+1;
    }
    int* sort = new int[numNets];
    for (i = 0; i<numNets; i++){
        sort[i] = i+1;
    }
    int* left_best = new int[numOfBlocks/2];
    int* right_best = new int[numOfBlocks/2];
    sorting (Blocks, &order);
    sort_netnums(Nets, &sort, numNets);
//    for(i = 0; i<numOfBlocks; i++){
//        int numberOfNet = Blocks[order[i]-1].GetNumOfNets();
//        cout<<order[i]<<"::"<<numberOfNet<<endl;
//    }
    InitialSol(Nets,&left_best, &right_best, numOfBlocks, numNets, sort);
    
    int lb_best = lowerBound_initial(Nets, left_best, right_best, numNets, numOfBlocks);
    cout<<"lb of initial before swap :"<<lb_best<<endl;
    
    
   Initial_solution_swap(Nets, &left_best, &right_best, numOfBlocks, Blocks);

    
 
    lb_best = lowerBound_initial(Nets, left_best, right_best, numNets, numOfBlocks);
    cout<<"lb of initial:"<<lb_best<<endl;
    int node_count =0;
    
    data* newNode = new data;
    if( caseDirection == 1){
    newNode = makeTree(Blocks, 0, NULL, PARENT, order, numOfBlocks, &lb_best, &left_best,&right_best, &node_count, numNets);
    }
   
    
    //////parallel logic starts here//
 

    if(caseDirection ==2){
    threadParams* Params = new threadParams;
    //data* newNode = new data;
    node_count = 1;
    newNode->blocknum = order[0];
    newNode->parent= NULL;
    newNode->RightOrLeftList = LEFT_CHILD;
    newNode->runningLBsum = 0;
	//load Params with an initial value that you use to call the first make tree function
    Params->Parent = newNode;
    Params->index = 1;
    Params->order = order;
    Params->numOfBlocks = numOfBlocks;
    Params->lb_best = &lb_best;
    Params->left = &left_best;
    Params->right = &right_best;
    Params->count_node = &node_count;
    Params->numNets = numNets;
    Params->Blocks = Blocks;
	//etc, etc...
    //cout<<order[0];
	pthread_t BBthread;
	pthread_create(&BBthread, NULL, makeTreeParallel, (void*)Params);
	pthread_join(BBthread, NULL);
       //data* newNode =  
   
   }

    lb_best = lowerBound_initial(Nets, left_best, right_best, numNets, numOfBlocks);
    cout<<"LB after tree"<<lb_best<<endl;

    cout<<"node visited"<<node_count<<endl;
   // cout<<newNode->left->blocknum<<endl;
    
    
    // keep everything above this.////
    if (Nets){
        delete[] Nets;
    }
    
    //cout<<numOfBlocks<<"::"<<newNode;
    
    DrawOnScreen(newNode, numOfBlocks);
    
    cout<<"Done"<<endl;
    return 0;
}
