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


void* makeTreeParallel(void* ptr) {
    pthread_mutex_init(&mutex1, NULL);
    
    threadParams* Params = (threadParams*) ptr;
    data* Parent = Params->Parent;
    int index = Params->index;
    int* order = Params->order;
    int numOfBlocks = Params->numOfBlocks;
    int* lb_best = Params->lb_best; // global	
    int** left = Params->left; // global
    int** right = Params->right; // global
    int* count_node = Params->count_node; // global 
    int numNets = Params->numNets;
    vector<block> Blocks = Params->Blocks;

    //data* newNode_right = new data;
    cout<<"index-"<<index<<endl;
    pthread_t leftChildThread;
    threadParams* LeftChildParams = new threadParams;
    data* newNode = new data;
    newNode->blocknum = order[index];
    newNode->RightOrLeftList = LEFT_CHILD;
    int lb = calculateCO(newNode, Blocks, numNets);
    newNode->runningLBsum = lb;
    pthread_mutex_lock(&mutex1);
    *count_node = *count_node + 1; 
    cout<<"Left-"<<order[index]<<endl;
    cout<<"lb for left nodes"<<newNode->runningLBsum<<endl;// cant figure this one out
    pthread_mutex_unlock(&mutex1);
    
    if (newNode->runningLBsum >= *lb_best) {
        Params->Parent->left = NULL;
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
        Params->Parent->left =  NULL;
        
        //return NULL;// dont make the thread// instead of null.
    }

    
    LeftChildParams->Parent = newNode;
    LeftChildParams->index = index + 1;
    LeftChildParams->order = order;
    LeftChildParams->numOfBlocks = numOfBlocks;
    LeftChildParams->lb_best = lb_best;
    LeftChildParams->left = left;
    LeftChildParams->right = right;
    LeftChildParams->count_node = count_node;
    LeftChildParams->numNets = numNets;
    LeftChildParams->Blocks = Blocks;
    
    if(leftcount<= numOfBlocks/2 && newNode->runningLBsum < *lb_best && index+1 < numOfBlocks){
        pthread_t BBthread;
        Params->Parent->left = newNode;
	pthread_create(&BBthread, NULL, makeTreeParallel, (void*)LeftChildParams);
    }


    data* newNode_right = new data;
    newNode_right->parent = Parent;
    newNode_right->blocknum = order[index];
    newNode_right->RightOrLeftList = RIGHT_CHILD;
    int lb_right = calculateCO(newNode_right, Blocks, numNets);
    newNode_right->runningLBsum = lb_right;
    
    pthread_mutex_lock(&mutex1);
    *count_node = *count_node + 1; 
     cout<<"right-"<<order[index]<<endl;
    cout<<"lb for right nodes"<<newNode_right->runningLBsum<<endl;// cant figure this one out
    pthread_mutex_unlock(&mutex1);
    
   
    if (newNode_right->runningLBsum >= *lb_best) {
        Params->Parent->left = newNode;
        return NULL;
    }
    data* node_right = newNode_right;
    int rightcount = 0;
    while (node_right != NULL) {
        if (node_right->RightOrLeftList == RIGHT_CHILD) {
            rightcount++;
        }
        node_right = node_right->parent;
    }
    if (rightcount > numOfBlocks / 2) {
        Params->Parent->right = NULL;
        return NULL;
    }
    
    threadParams* RightChildParams = new threadParams;
    RightChildParams->Parent = newNode_right;
    RightChildParams->index = index + 1;
    RightChildParams->order = order;
    RightChildParams->numOfBlocks = numOfBlocks;
    RightChildParams->lb_best = lb_best;
    RightChildParams->left = left;
    RightChildParams->right = right;
    RightChildParams->count_node = count_node;
    RightChildParams->numNets = numNets;
    RightChildParams->Blocks = Blocks;
    
    pthread_mutex_lock(&mutex1);

    //any thread changing global variable read/write happens here
    if (index + 1 >= numOfBlocks) {
        int k = 0;
        int l = 0;
        data* newnode_left = newNode;
        data* newnode_right = newNode_right;
        if(leftcount == numOfBlocks/2 && rightcount == numOfBlocks/2){
            cout << "lb:" << newnode_left->runningLBsum;
            if (newnode_left->runningLBsum < *lb_best) {
                *lb_best = newnode_left->runningLBsum;
                cout << endl << "lbBest" << lb << endl;
                while (newnode_left != NULL) {
                    if (newnode_left->RightOrLeftList == LEFT_CHILD) {
                        (*right)[k] = newnode_left->blocknum;
                        k++;
                    }
                    if (newnode_left->RightOrLeftList == RIGHT_CHILD) {
                        (*left)[l] = newnode_left->blocknum;
                        l++;
                    }
                    newnode_left = newnode_left->parent;

                }
            }

            if (newnode_right->runningLBsum < *lb_best) {
                *lb_best = newNode->runningLBsum;
                cout << endl << "lbBest" << lb << endl;
                while (newnode_right != NULL) {
                    if (newnode_right->RightOrLeftList == LEFT_CHILD) {
                        (*right)[k] = newnode_right->blocknum;
                        k++;
                    }
                    if (newnode_right->RightOrLeftList == RIGHT_CHILD) {
                        (*left)[l] = newnode_right->blocknum;
                        l++;
                    }
                    newnode_right = newnode_right->parent;

                }
            }
        }
        
        return NULL;

    }
    pthread_mutex_unlock(&mutex1);
    
    if(rightcount<= numOfBlocks/2 && newNode_right->runningLBsum < *lb_best && index+1 < numOfBlocks){
        pthread_t BBthread;
        Params->Parent->right = newNode_right;
	void* ret = makeTreeParallel((void*)RightChildParams);
    }
    
    return NULL;
}

