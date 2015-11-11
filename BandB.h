
#ifndef PLACER_H
#define	PLACER_H

#include <iostream>
#include <vector>

using namespace::std;

class block{
private:
    int blockNum;
    vector<int> netnum;
    int netnums;
    bool lock = false;
    
public:
    void setBlockNum(int _blockNum){
        blockNum = _blockNum;
    }
    int getBlockNum(){
        return blockNum;
    }
    vector<int>* getNetNum(){
        return &netnum;
    }
    void setnetnums(int a){
        netnums = a;
    }
    int GetNumOfNets(){
        return netnums;
    }
    void setlock(bool a){
        lock = a;
        
    }
    bool getlock(){
        return lock;
    }
};


class Net{
private:
    int numPins;
    vector<int> BlockNums;
public:
    Net(){
        numPins = 0;
    }
     void incrementNumPins(){
        numPins++;
    }
    void setNumPins(int _numPins){
        numPins = _numPins;
    }
    int getNumPins(){
        return numPins;
    }
    
    vector<int>* getBlockNums(){
        return &BlockNums;
    }
    
};

struct swapvar{
    int index;
    int LorR;
};

struct data{
    //all the important relevant data goes here!
    int blocknum;
    int RightOrLeftList;
    int runningLBsum; 
    data* left; //left child
    data* right; //right child
    data* parent; //parent (for head this is obviously null)
};






struct threadParams {
	//all the parameters that were being passed into the makeTree function HAVE to be put in here, with the exact same data-type
	vector<block> Blocks; 
	int index; 
	data* Parent; 
	int* order; 
	int numOfBlocks; 
	int* lb_best; // global	
	int** left;  // global
	int** right;  // global
	int* count_node; // global 
	int numNets;
	//etc, etc...

};

struct LBandNode{ //couldnt think of any other name, plus coming close to St.George
	data* node;
	int LB;
}

#endif	/* PLACER_H */

