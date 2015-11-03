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


//// sorting based on highest number nets connected to/////
void sorting(vector<block> Blocks, int** order){
    int size = Blocks.size();
    for (int i =0; i<size; i++){
        int max = Blocks[(*order)[i]-1].GetNumOfNets();
        for (int j = i+1; j<size; j++){
            if(max< Blocks[(*order)[j]-1].GetNumOfNets()){
               max = Blocks[(*order)[j]-1].GetNumOfNets();
               int swap = (*order)[i];
               (*order)[i]=(*order)[j];
               (*order)[j]=swap;
            }
        }
    }
}
/// sorting done//


/// get initial solution///

void InitialSol(Net* nets, int** left, int** right, int numOfBlocks, int numNets){
    int leftnum = 0;
    int rightnum =0;
    for (int i = 0; i < numNets; i++) {
        vector<int>* blocknums = nets[i].getBlockNums();
        for (int j = 0; j < blocknums->size(); j++) {
            int leftcount = 0;
            int rightcount =0;
            if (leftnum < numOfBlocks / 2) {
                for (int y = 0; y < leftnum; y++) {
                    if ((*left)[y] == (*blocknums)[j]) {
                        leftcount++;
                    }
                }
                if (leftcount == 0) {
                    (*left)[leftnum] =(*blocknums)[j];
                    leftnum++;
                }
            }
            else{

                for (int y = 0; y <rightnum; y++) {
                    if ((*right)[y] == (*blocknums)[j]){
                        rightcount++;
                    }
                }
                for(int k = 0; k<numOfBlocks/2; k++){
                    if((*left)[k]== (*blocknums)[j]){
                        rightcount++;
                    }
                }
                if (rightcount == 0) {
                    (*right)[rightnum] = (*blocknums)[j];
                    rightnum++;
                }
            }
        }
    }
}
/// got initial solution ////

//// to get the lower bound for initial solution.////
int lowerBound_initial(Net* nets, int* left, int* right, int numNets, int numOfBlocks){
    int lb = 0;
    for (int i = 0; i < numNets; i++) {
        int leftcount = 0;
        int rightcount = 0;
        vector<int>* blocknums = nets[i].getBlockNums();
        for(int k = 0; k<blocknums->size(); k++){
            for(int j = 0; j<numOfBlocks/2; j++){
                if((*blocknums)[k]== left[j]){
                    leftcount++;
                }
            }
            for(int j = 0; j<numOfBlocks/2; j++){
                if((*blocknums)[k]== right[j]){
                    rightcount++;
                }
            }            
        }
        if(leftcount!=0 && rightcount !=0){
            cout<<"netnum::"<<i+1<<endl;
            lb++;
        }
    }  
    return lb;
}
// lower bound for initial made.//////

