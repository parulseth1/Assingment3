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
#define FILENAME "/home/parul/NetBeansProjects/BandBPartition/cct2.txt"

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

        cout<<i<<"::"<<Nets[i].getNumPins()<<endl;
    }
    int i;
    int* order = new int[numOfBlocks];
    for (i = 0; i<numOfBlocks; i++){
        order[i] = i+1;
    }
    int* left = new int[numOfBlocks/2];
    int* right = new int[numOfBlocks/2];
    sorting (Blocks, &order);
    InitialSol(Nets,&left, &right, numOfBlocks, numNets);
    
    return 0;
}

