#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Partition.h"
#include "BandB.h"
#include <limits.h>
//#define INTMIN -100000002882993993002882
#define LEFT_CHILD 1
#define RIGHT_CHILD 2
#define PARENT (-1)
#include <pthread.h>
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barr;
//int* lb_best_global;

void* makeTreeParallel(void* ptr) {
    pthread_mutex_init(&mutex1, NULL);
    
    
    
    
    threadParams* Params = (threadParams*) ptr;
    //pthread_barrier_init(&barr, NULL, 2^(Params->numOfBlocks) -2);
    data* Parent = Params->Parent;
    int index = Params->index;
    int* order = Params->order;
    int numOfBlocks = Params->numOfBlocks;
    int* lb_best_global = Params->lb_best; // global	
    int** left = Params->left; // global
    int** right = Params->right; // global
    int* count_node = Params->count_node; // global 
    int numNets = Params->numNets;
    vector<block> Blocks = Params->Blocks;
    
    pthread_mutex_lock(&mutex1);
        if (index == numOfBlocks) {
        int k = 0;
        int l = 0;
        data* newnode = Parent;
            //cout << "lb:" << newnode->runningLBsum;
            int lb = calculateCO(Parent, Blocks, numNets);
            if (lb < *lb_best_global) {
                *lb_best_global = newnode->runningLBsum;
                //cout << endl << "lbBest" << *lb_best_global << endl;
                while (newnode != NULL) {
                    //cout<<"while stuck"<<endl;
                    if (newnode->RightOrLeftList == LEFT_CHILD) {
                        (*right)[k] = newnode->blocknum;
                        k++;
                        
                    }
                    if (newnode->RightOrLeftList == RIGHT_CHILD) {
                        (*left)[l] = newnode->blocknum;
                        l++;
                    }
                    newnode = newnode->parent;

                }
            }
        pthread_mutex_unlock(&mutex1);
        return NULL;
    }
    
   pthread_mutex_unlock(&mutex1);
    

    //data* newNode_right = new data;
    //cout<<"index-"<<index<<endl;
    pthread_t leftChildThread;
    threadParams* LeftChildParams = new threadParams;
    data* newNode = new data;
    newNode->parent = Parent;
    newNode->blocknum = order[index];
    newNode->RightOrLeftList = LEFT_CHILD;
    int lb = calculateCO(newNode, Blocks, numNets);
    newNode->runningLBsum = lb;
    pthread_mutex_lock(&mutex1);
    *count_node = *count_node + 1; 
    pthread_mutex_unlock(&mutex1);
    
    if (newNode->runningLBsum >= *lb_best_global) {
        newNode->left = NULL;
        newNode->right =NULL;
        Params->Parent->left = newNode;
        //return NULL;// dont make the thread// instead of null.
    }
    data* node = newNode;
    int leftcount = 0;
    while (node != NULL) {
        if (node->RightOrLeftList == LEFT_CHILD) {
            leftcount++;
        }
        node = node->parent;
    }
    if (leftcount > numOfBlocks / 2) {
        newNode->left = NULL;
        newNode->right =NULL;
        Params->Parent->left = newNode;
        
        
        //return NULL;// dont make the thread// instead of null.
    }

    
    LeftChildParams->Parent = newNode;
    LeftChildParams->index = index + 1;
    LeftChildParams->order = order;
    LeftChildParams->numOfBlocks = numOfBlocks;
    LeftChildParams->lb_best = lb_best_global;
    LeftChildParams->left = left;
    LeftChildParams->right = right;
    LeftChildParams->count_node = count_node;
    LeftChildParams->numNets = numNets;
    LeftChildParams->Blocks = Blocks;
    
    if(leftcount<= numOfBlocks/2 && newNode->runningLBsum < *lb_best_global){
        pthread_t BBthread;
	pthread_create(&BBthread, NULL, makeTreeParallel, (void*)LeftChildParams);
        Params->Parent->left = LeftChildParams->Parent;
        pthread_join(BBthread, NULL);
        
    }
    


    data* newNode_right = new data;
    newNode_right->parent = Parent;
    newNode_right->blocknum = order[index];
    newNode_right->RightOrLeftList = RIGHT_CHILD;
    int lb_right = calculateCO(newNode_right, Blocks, numNets);
    newNode_right->runningLBsum = lb_right;
    
    pthread_mutex_lock(&mutex1);
    *count_node = *count_node + 1; 

    pthread_mutex_unlock(&mutex1);
    
   
    if (newNode_right->runningLBsum >= *lb_best_global) {
        newNode_right->left = NULL;
        newNode_right->right = NULL;
        Params->Parent->right = newNode_right;
        //pthread_exit(NULL);
        return NULL;
    }
    data* node_right = newNode_right;
    int rightcount = 0;
    while (node_right != NULL) {
        //cout<<"while "
        if (node_right->RightOrLeftList == RIGHT_CHILD) {
            rightcount++;
            //cout<<rightcount<<endl;
        }
        node_right = node_right->parent;
    }
    if (rightcount > numOfBlocks / 2) {
        newNode_right->left = NULL;
        newNode_right->right = NULL;
        Params->Parent->right = newNode_right;
        
        //pthread_exit(NULL);
        return NULL;
    }
    
    threadParams* RightChildParams = new threadParams;
    RightChildParams->Parent = newNode_right;
    RightChildParams->index = index + 1;
    RightChildParams->order = order;
    RightChildParams->numOfBlocks = numOfBlocks;
    RightChildParams->lb_best = lb_best_global;
    RightChildParams->left = left;
    RightChildParams->right = right;
    RightChildParams->count_node = count_node;
    RightChildParams->numNets = numNets;
    RightChildParams->Blocks = Blocks;
    
  


    if(rightcount<= numOfBlocks/2 && newNode_right->runningLBsum < *lb_best_global){
        
	void* ret = makeTreeParallel((void*)RightChildParams);
        Params->Parent->right = RightChildParams->Parent;
    }
    
    
    //pthread_barrier_wait(&barr);
    return NULL;
}


