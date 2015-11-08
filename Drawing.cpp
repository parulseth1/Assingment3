#include "Drawing.h"

#include <iostream>

using namespace::std;

void drawscreen();

double* x = NULL;
double* y = NULL;
int gridSize = 10;
int blockCnt = 0;
Net* nets = NULL;
int netCnt = 0;
vector<block> Blocks;

int LoadBlocks (double* _x, double* _y, int _blockCnt, int _gridSize) {

    x = _x;
    y = _y;
    blockCnt = _blockCnt;
    gridSize = _gridSize;
    
    for (int i = 0; i < blockCnt; i++){
        x[i] = x[i]/10.0;
        y[i] = y[i]/10.0;
    }
}

int LoadRatsNest(Net* _nets, int _netCnt) {
    nets = _nets;
    netCnt = _netCnt;
}

int DrawOnScreen() {
    init_graphics("Analytical Placer", WHITE);

    set_visible_world(0, 0, 1200, 1200);

    event_loop(NULL, NULL, NULL, drawscreen);

    close_graphics();
    cout<<"END";
    return 0;
}

void drawscreen() {
    //draw the main grid and the wire mesh - gridSize x gridSize grid i.e.
    //draw the main rect first
    clearscreen();
    float blockDim = 100;//1000 / (gridSize + 2);
    float rectOffset = 100;

    setcolor(135, 206, 250); //blue
    fillrect(rectOffset, rectOffset, rectOffset + blockDim*gridSize, rectOffset + blockDim * gridSize);

    //draw the mesh lines
    setcolor(0, 0, 0); //black
    for (int i = 0; i <= gridSize; i++) {
        drawline(rectOffset + blockDim*i, rectOffset, rectOffset + blockDim*i, rectOffset + gridSize * blockDim);
        drawline(rectOffset, rectOffset + blockDim*i, rectOffset + gridSize*blockDim, rectOffset + blockDim * i);
    }

    //draw the blocks as points
    setcolor(0, 0, 0); //grey
    for (int i = 0; i < blockCnt; i++) {
        drawline(rectOffset + blockDim * (x[i]), rectOffset + blockDim * (y[i]), rectOffset + blockDim * (x[i]), rectOffset + blockDim * (y[i]));
    }

    //draw the rats nest
    setcolor(255, 0, 0);
    //for each net 
    if (nets != NULL){
        for (int i = 0; i < netCnt; i++){
            vector<int>* blocks = nets[i].getBlockNums();
            for (int j = 0; j < blocks->size(); j++){
                for (int k = j + 1; k < blocks->size(); k++){
                    double x1 = x[(*blocks)[j]-1];
                    double y1 = y[(*blocks)[j]-1];
                    double x2 = x[(*blocks)[k]-1];
                    double y2 = y[(*blocks)[k]-1];
                    drawline(rectOffset + blockDim*x1, rectOffset + blockDim*y1, rectOffset + blockDim*x2, rectOffset + blockDim*y2);
                }
            }
        }
    }
    //	
}
