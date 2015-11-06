
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
    int gain = 0;
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
    int getGain(){
        return gain;
    }
    void setGain(int a){
        gain = gain + a;
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
#endif	/* PLACER_H */

