#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Partition.h"
#include "BandB.h"
#define INTMIN -100000002882993993002882
#define LEFT_CHILD 1
#define RIGHT_CHILD 2
#define PARENT (-1)
//#define PRUNE 
// first we got to make function to make initial solution
// get the LB function/// how????????
// then define the ordering.


//// sorting based on highest number nets connected to/////
int sol_reached;
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
void sort_netnums(Net* nets, int** sort, int numnet){
    for(int i = 0; i<numnet; i++){
        int max = nets[(*sort)[i]-1].getNumPins();
        for (int j = i + 1; j < numnet; j++) {
            if (max < nets[(*sort)[j] - 1].getNumPins()) {
                max = nets[(*sort)[j] - 1].getNumPins();
                int swap = (*sort)[i];
                (*sort)[i] = (*sort)[j];
                (*sort)[j] = swap;
            }
        }
    }
}

/// get initial solution///

void InitialSol(Net* nets, int** left, int** right, int numOfBlocks, int numNets, int*sort) {
    int leftnum = 0;
    int rightnum = 0;
    for (int i = 0; i < numNets; i++) {
        vector<int>* blocknums = nets[sort[i]-1].getBlockNums();
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
                if ((*blocknums)[k] == right[j]) {
                    rightcount++;
                }
            }
        }
        if (leftcount != 0 && rightcount != 0) {
            lb++;
        }
    }
    return lb;
}
// lower bound for initial made.//////


/// Kernighan - Lin algorithm to get a better initial solution///
void Initial_solution_swap(Net* nets, int** left, int** right, int numOfBlocks, vector<block> Blocks) {
    int G = 0;
    int K;
    int max_g;
    int* g_new = new int[numOfBlocks / 2];
    int num =0;
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
                if (i != k) {
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
        }
        int* Ivalues = new int[numOfBlocks];
        int* Evalues = new int[numOfBlocks];
        int* Dvalues = new int[numOfBlocks];
        for (int a = 0; a < numOfBlocks; a++) {
            Ivalues[a] = 0;
            Evalues[a] = 0;
        }
        for (int a = 0; a < numOfBlocks; a++) {
            for (int b = 0; b < numOfBlocks/2; b++) {
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
                if(a!=b){
                    matrix_g[a][b] = Dvalues[a] + Dvalues[b];
                    matrix_g[a][b] = matrix_g[a][b] - (2 * (matrix[a][b]));
                }
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
            max = INTMIN;
            for (int a = 0; a < numOfBlocks/2; a++) {
                for (int b = 0; b < numOfBlocks/2; b++) {
                    if (Blocks[(*left)[a]-1].getlock() == false && Blocks[(*right)[b]-1].getlock() == false) {
                        if (matrix_g[(*left)[a]-1][(*right)[b]-1] > max) {
                            max = matrix_g[(*left)[a]-1][(*right)[b]-1];
                            g = matrix_g[(*left)[a]-1][(*right)[b]-1];
                            if (n != 0) {
                                g_new[n] = g + g_new[n - 1];
                            } else {
                                g_new[n] = g;
                            }
                            swap1[n] = a+1;
                            swap2[n] = b+1;
                            
                        }
                    }
                }
            }          
            Blocks[(*left)[swap1[n]-1]-1].setlock(true);
            Blocks[(*right)[swap2[n]-1]-1].setlock(true);
           
            for(int r =0; r<numOfBlocks; r++){
                for(int o =0; o<numOfBlocks/2; o++){
                    if(r+1 == (*left)[o] && r+1 !=swap1[n]){
                        Dvalues[r] = Dvalues[r] +(2* matrix[r][((*left)[swap1[n]-1])-1]) - (2* matrix[r][((*right)[swap2[n]-1])-1]);
                    }
                    if(r+1 == (*right)[o] && r+1 !=swap2[n]){
                        Dvalues[r] = Dvalues[r] - (2*matrix[r][((*left)[swap1[n]-1])-1]) + (2*matrix[r][((*right)[swap2[n]-1])-1]);
                    }
                }
            }
            
            for (int a = 0; a < numOfBlocks; a++) {
                for (int b = 0; b < numOfBlocks; b++) {
                    matrix_g[a][b] = Dvalues[a] + Dvalues[b];
                    matrix_g[a][b] = matrix_g[a][b] - (2 * (matrix[a][b]));
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
                    one = (*left)[swap1[i]- 1];
                    two = (*right)[swap2[i] - 1];
                    (*left)[swap1[i]-1] = two;
                    (*right)[swap2[i]-1] = one;
        }
        for(int w=0; w<numOfBlocks; w++){
            Blocks[w].setlock(false);
        }
        G = g_new[K];
        num++;
    } while (G> 0);


}

// the big ass initial solution is over.....///// finally ///  functionality checked//
// decision tree.. going for it!!//
data* makeTree(vector<block> Blocks, int index, data* Parent, int LeftOrRightChild, int* order, int numOfBlocks, int* lb_best, int** left, int** right, int* left_count, int* right_count, int* count_node, int numNets){
    // a condition based on which the node will not be created and a null is returned,indicates the end of the tree
    
    if (index >= numOfBlocks){
        int k=0;
        int l=0;
        data* newnode = Parent;
        *lb_best = Parent->runningLBsum;
        while(newnode!=NULL){
            if(newnode->RightOrLeftList == LEFT_CHILD){
                 (*right)[k] = newnode->blocknum;
                 k++;
                } 
            if(newnode->RightOrLeftList == RIGHT_CHILD){
                 (*left)[l] = newnode->blocknum;
                 l++;
                } 
         newnode = newnode->parent; 
         
        }
        //calculate final LB compare with LB_best and put the solution in left and right;
        return NULL;
        
    }
    data* newNode = new data; //the new node is created here
    cout<<order[index]<<endl;
    //based on whether this is the left/right child, we can do diff operations,
    if (LeftOrRightChild == PARENT){
        newNode->blocknum = order[index];
        newNode->RightOrLeftList=LEFT_CHILD;
        *left_count = *left_count +1;
        newNode->runningLBsum = 0;
        // put in a side
        // set LB =0 
        // store and compare//
    }
    //left adds, right subtracts, but replace with whatever necessary
    else if (LeftOrRightChild == LEFT_CHILD){
        
        newNode->blocknum = order[index];
        newNode->RightOrLeftList = LeftOrRightChild;
        int lb =calculateCO(newNode,Blocks,numNets);
        newNode->runningLBsum =lb;
        
        *count_node = *count_node+1;
        *left_count = *left_count+1;
        cout<<"leftcount"<<*left_count<<endl;
        if(newNode->runningLBsum >= *lb_best){
            return NULL;
        }
        if(*left_count > numOfBlocks/2){
            return NULL;
        }

        
    }
    else if (LeftOrRightChild == RIGHT_CHILD){
        newNode->blocknum = order[index];
        newNode->blocknum = order[index];
        newNode->RightOrLeftList = LeftOrRightChild;
        int lb =calculateCO(newNode,Blocks,numNets);
        newNode->runningLBsum =lb;
        
        *count_node = *count_node+1;
        *right_count = *right_count+1;
        if(newNode->runningLBsum >= *lb_best){
            return NULL;
        }
        if(*right_count > numOfBlocks/2){
            return NULL;
        }
        
        
    }
    
    //set up the pointers,
    newNode->parent = Parent;
    newNode->left = makeTree(Blocks, index+1, newNode, LEFT_CHILD, order, numOfBlocks,lb_best, left, right, left_count, right_count, count_node, numNets); //the left child is a new node, that is recursively created by the maketree function
    newNode->right = makeTree(Blocks, index+1, newNode, RIGHT_CHILD, order, numOfBlocks,lb_best,left, right, left_count, right_count, count_node, numNets); //similarly for the right child
    
    
    return newNode;
}

int calculateCO(data* node, vector<block> Blocks, int numNets){
     int LB = 0;
     int* accounted = new int[numNets];
     for(int i= 0; i<numNets; i++){
     accounted[i] = 0;
     }
     data* newnode = node;
     vector<int>* netnum_node = Blocks[(node->blocknum) -1].getNetNum();
     while(node!= NULL){
     while(newnode->parent != NULL){
         if(node->RightOrLeftList != newnode->parent->RightOrLeftList){
            vector<int>* netnum_parent = Blocks[(newnode->parent->blocknum)-1].getNetNum();
            for(int i =0; i<netnum_node->size(); i++){
                for(int j = 0; j<netnum_parent->size(); j++){
                    if(netnum_node[i]==netnum_parent[j]){
                        if(accounted[(*netnum_node)[i] - 1] == 0){
                            LB++;
                            accounted[(*netnum_node)[i] - 1] = 1;
                        } 
                    }
                }
                
            }
         }
         newnode = newnode->parent;
     }
     node = newnode->parent;
     }
     //LB = LB+node->parent->runningLBsum;
     return LB;
 }
//// finally its done!!!!!!!//// yayyyyyyieeeeee/////


