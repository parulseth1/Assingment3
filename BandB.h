
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


#endif	/* PLACER_H */

