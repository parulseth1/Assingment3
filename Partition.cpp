#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Partition.h"
#include "BandB.h"

// first we got to make function to make initial solution
// get the LB function/// how????????
// then define the ordering.

void sorting(vector<block> Blocks, int* order){
    int size = Blocks.size();
    for (int i =0; i<size; i++){
        int max = Blocks[order[i]-1].GetNumOfNets();
        for (int j = i+1; j<size; j++){
            if(max< Blocks[order[j]-1].GetNumOfNets()){
               max = Blocks[order[j]-1].GetNumOfNets();
               int swap = order[i];
               order[i]=order[j];
               order[j]=swap;
            }
        }
    }
}

void InitialSol(){
    
}