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
#define FILENAME "/home/parul/NetBeansProjects/BandBPartition/cct3.txt"
#define PARENT (-1)

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    vector<block> Blocks;
    int numOfBlocks;
    Net* Nets = NULL;
    int numNets = 0;
    doParse(FILENAME, &Blocks, &Nets, &numNets);
    
    cout<<"Parsing Complete"<<endl;
    cout<<"number of nets: "<<numNets<<endl;
    numOfBlocks = Blocks.size();
    for (int i = 0; i < numNets; i++){

        cout<<i+1<<"::"<<Nets[i].getNumPins()<<endl;
    }
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
    for(i = 0; i<numOfBlocks; i++){
        int numberOfNet = Blocks[order[i]-1].GetNumOfNets();
        cout<<order[i]<<"::"<<numberOfNet<<endl;
    }
    InitialSol(Nets,&left_best, &right_best, numOfBlocks, numNets, sort);
    
    int lb_best = lowerBound_initial(Nets, left_best, right_best, numNets, numOfBlocks);
    cout<<"lb of initial before swap :"<<lb_best<<endl;
    
    Initial_solution_swap(Nets, &left_best, &right_best, numOfBlocks, Blocks);
 
    lb_best = lowerBound_initial(Nets, left_best, right_best, numNets, numOfBlocks);
    cout<<"lb of initial:"<<lb_best<<endl;
    int leftcount =0;
    int rightcount=0;
    int node_count =0;

    data* newNode = makeTree(Blocks, 0, NULL, PARENT, order, numOfBlocks, &lb_best, &left_best,&right_best, &leftcount, &rightcount, &node_count, numNets);
    
    
    
    
    
    // keep everything above this.////
    if (Nets){
        delete[] Nets;
    }
    
    cout<<"Done"<<endl;
    return 0;
}

