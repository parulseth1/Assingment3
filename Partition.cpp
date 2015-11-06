#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Partition.h"
#include "BandB.h"
#define INTMIN 0
// first we got to make function to make initial solution
// get the LB function/// how????????
// then define the ordering.


//// sorting based on highest number nets connected to/////

void sorting(vector<block> Blocks, int** order) {
    int size = Blocks.size();
    for (int i = 0; i < size; i++) {
        int max = Blocks[(*order)[i] - 1].GetNumOfNets();
        for (int j = i + 1; j < size; j++) {
            if (max < Blocks[(*order)[j] - 1].GetNumOfNets()) {
                max = Blocks[(*order)[j] - 1].GetNumOfNets();
                int swap = (*order)[i];
                (*order)[i] = (*order)[j];
                (*order)[j] = swap;
            }
        }
    }
}
/// sorting done//


/// get initial solution///

void InitialSol(Net* nets, int** left, int** right, int numOfBlocks, int numNets) {
    int leftnum = 0;
    int rightnum = 0;
    for (int i = 0; i < numNets; i++) {
        vector<int>* blocknums = nets[i].getBlockNums();
        for (int j = 0; j < blocknums->size(); j++) {
            int leftcount = 0;
            int rightcount = 0;
            if (leftnum < numOfBlocks / 2) {
                for (int y = 0; y < leftnum; y++) {
                    if ((*left)[y] == (*blocknums)[j]) {
                        leftcount++;
                    }
                }
                if (leftcount == 0) {
                    (*left)[leftnum] = (*blocknums)[j];
                    leftnum++;
                }
            } else {

                for (int y = 0; y < rightnum; y++) {
                    if ((*right)[y] == (*blocknums)[j]) {
                        rightcount++;
                    }
                }
                for (int k = 0; k < numOfBlocks / 2; k++) {
                    if ((*left)[k] == (*blocknums)[j]) {
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

int lowerBound_initial(Net* nets, int* left, int* right, int numNets, int numOfBlocks) {
    int lb = 0;
    for (int i = 0; i < numNets; i++) {
        int leftcount = 0;
        int rightcount = 0;
        vector<int>* blocknums = nets[i].getBlockNums();
        for (int k = 0; k < blocknums->size(); k++) {
            for (int j = 0; j < numOfBlocks / 2; j++) {
                if ((*blocknums)[k] == left[j]) {
                    leftcount++;
                }
            }
            for (int j = 0; j < numOfBlocks / 2; j++) {
                if ((*blocknums)[k] == right[j]) {
                    rightcount++;
                }
            }
        }
        if (leftcount != 0 && rightcount != 0) {
            cout << "netnum::" << i + 1 << endl;
            lb++;
        }
    }
    return lb;
}
// lower bound for initial made.//////

void Initial_solution_swap(Net* nets, int** left, int** right, int numNets, int numOfBlocks, int lb_best, vector<block> Blocks) {
    int G = 0;
    int K;
    int max_g;
    int* g_new = new int[numOfBlocks / 2];
    do {
        int** matrix = NULL;
        matrix = new int* [numOfBlocks];
        for (int i = 0; i < numOfBlocks; i++) {
            matrix[i] = new int[numOfBlocks];
        }
        for (int i = 0; i < numOfBlocks; i++) {
            for (int k = 0; k < numOfBlocks; k++) {
                matrix[i][k] = 0;
            }
        }
        for (int i = 0; i < numOfBlocks; i++) {
            vector<int>* netnum_i = Blocks[i].getNetNum();
            for (int k = 0; k < numOfBlocks; k++) {
                if (i == k) {
                    matrix[i][k] = 0;
                }
                vector<int>* netnum_k = Blocks[k].getNetNum();
                for (int h = 0; h < netnum_i->size(); h++) {
                    for (int n = 0; n < netnum_k->size(); n++) {
                        if ((*netnum_i)[h] == (*netnum_k)[n]) {
                            matrix[i][k] = matrix[i][k] + 1;
                        }
                    }
                }
            }
        }
        int* Ivalues = new int[numOfBlocks];
        int* Evalues = new int[numOfBlocks];
        int* Dvalues = new int[numOfBlocks];
        for (int a = 0; a < numOfBlocks; a++) {
            Ivalues[a] = 0;
            Evalues[a] = 0;
        }
        for (int a = 0; a < numOfBlocks; a++) {
            for (int b = 0; b < numOfBlocks; b++) {
                Ivalues[a] = Ivalues[a] + matrix[a][(*left)[b] - 1];
                Evalues[a] = Evalues[a] + matrix[a][(*right)[b] - 1];
            }
            Dvalues[a] = Evalues[a] - Ivalues[a];
        }

        int** matrix_g = NULL;
        matrix_g = new int* [numOfBlocks];
        for (int i = 0; i < numOfBlocks; i++) {
            matrix_g[i] = new int[numOfBlocks];
        }

        for (int a = 0; a < numOfBlocks; a++) {
            for (int b = 0; b < numOfBlocks; b++) {
                matrix_g[a][b] = Dvalues[a] + Dvalues[b];
                matrix_g[a][b] = matrix_g[a][b] - (2 * (matrix[a][b]));
            }
        }
        int g;
        
        int row, column;
        int* swap1 = new int[numOfBlocks / 2];
        int* swap2 = new int[numOfBlocks / 2];
        int max;
        swapvar* swap_var1 = new swapvar[numOfBlocks / 2];
        swapvar* swap_var2 = new swapvar[numOfBlocks / 2];

        for (int n = 0; n < numOfBlocks / 2; n++) {
            for (int a = 0; a < numOfBlocks; a++) {
                for (int b = 0; b < numOfBlocks; b++) {
                    if (Blocks[a].getlock() == false && Blocks[b].getlock() == false) {
                        if (matrix_g[a][b] > max) {
                            max = matrix_g[a][b];
                            g = matrix_g[a][b];
                            if (n != 0) {
                                g_new[n] = g + g_new[n - 1];
                            } else {
                                g_new[n] = g;
                            }
                            swap1[n] = a + 1;
                            swap2[n] = b + 1;
                            Blocks[a].setlock(1);
                            Blocks[b].setlock(1);
                        }
                    }
                }
            }
            for (int h = 0; h < numOfBlocks / 2; h++) {
                if (swap1[n] == (*left)[h]) {
                    swap_var1[n].index = h;
                    swap_var1[n].LorR = 1;
                }
                if (swap2[n] == (*left)[h]) {
                    swap_var2[n].index = h;
                    swap_var2[n].LorR = 1;
                }
                if (swap1[n] == (*right)[h]) {
                    swap_var1[n].index =h;
                    swap_var1[n].LorR = h;
                }
                if (swap2[n] == (*right)[h]) {
                    swap_var2[n].index = (*right)[h];
                    swap_var2[n].LorR = 0;
                }
            }
            if (swap_var1[n].LorR == 1) {
                if (swap_var2[n].LorR == 0) {
                   for(int r =0; r<numOfBlocks; r++){
                       for(int o =0; 0<numOfBlocks/2; o++){
                           if(r+1 == (*left)[o] && r+1 !=swap_var1[n].index){
                               Dvalues[r] = Dvalues[r] +(2*matrix[r][(*left)[swap_var1[n].index]-1]) - (2*matrix[r][(*right)[swap_var1[n].index]-1]);
                           }
                           if(r+1 == (*right)[o] && r+1 !=swap_var2[n].index){
                               Dvalues[r] = Dvalues[r] - (2*matrix[r][(*left)[swap_var1[n].index]-1]) + (2*matrix[r][(*right)[swap_var1[n].index]-1]);
                           }
                       }
                     
                   }
                }
            }
            if (swap_var1[n].LorR == 0) {
                if (swap_var2[n].LorR == 1) {
                   for(int r =0; r<numOfBlocks; r++){
                       for(int o =0; 0<numOfBlocks/2; o++){
                           if(r+1 == (*left)[o] && r+1 !=swap_var2[n].index){
                               Dvalues[r] = Dvalues[r] - (2*matrix[r][(*right)[swap_var1[n].index]-1]) + (2*matrix[r][(*left)[swap_var1[n].index]]);
                           }
                           if(r+1 == (*right)[o] && r+1 !=swap_var1[n].index){
                               Dvalues[r] = Dvalues[r] + (2*matrix[r][(*right)[swap_var1[n].index]-1]) + (2*matrix[r][(*left)[swap_var1[n].index]]);
                           }
                       }
                     
                   }
                }
            }
        }
        max_g = g_new[0];
        for(int q = 1; q<numOfBlocks/2; q++){
            if (max_g < g_new[q]){
                max_g = g_new[q];
                K = q;
            }
        }
        int one, two;
        for (int i = 0; i < K + 1; i++) {
            if (swap_var1[i].LorR == 1) {
                if (swap_var2[i].LorR == 0) {
                    one = (*left[swap_var1[i].index - 1]);
                    two = (*right[swap_var2[i].index - 1]);
                    (*left[swap1[0]]) = two;
                    (*right[swap2[0]]) = one;
                }
            }
            if (swap_var1[i].LorR == 0) {
                if (swap_var2[i].LorR == 1) {
                    one = (*left[swap_var2[i].index - 1]);
                    two = (*right[swap_var1[i].index - 1]);
                    (*left[swap1[0]]) = two;
                    (*right[swap2[0]]) = one;
                }
            }
        }
        for(int w=0; w<numOfBlocks; w++){
            Blocks[w].setlock(0);
        }
    } while (g_new[K] > 0);


}

// the big ass initial solution is over.....///// finally /// still have to check the functionality//




