
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
    //a variable here should indicate which list this belongs to and a cumulative LB
    int RightOrLeftList;
    int runningLBsum; 
    
    //THIS HAS TO BE SET TO ZERO IN THE CLASS CONSTRUCTOR!
    //this is the Binary tree stuff
    data* left; //left child
    data* right; //right child
    data* parent; //parent (for head this is obviously null)
};
#endif	/* PLACER_H */

