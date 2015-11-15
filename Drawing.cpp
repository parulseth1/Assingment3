#include "Drawing.h"

#include <iostream>
#include <cfloat>
#include <math.h>

using namespace::std;

#define LEFT 1
#define RIGHT 2

#define VERTICAL_FACTOR 5

struct Point{
    int x;
    int y;
    Point(int _x, int _y){
        x = _x;
        y = _y;
    }
};

void drawCircle(Point c, int r){
    drawarc(c.x, c.y, r, 0, 360);
}

void drawscreen();


data* head = NULL;
int levels = 0;
int circleRadius = 350;
Point ScreenSize(10000, 10000);
Point Parent(0, 0);
Point ParentCircle(0, 0);
int PixelsPerLevel = 0;
int initDistanceBetweenNodes = 0;
int Block_HL = 3*circleRadius;

int leftExpanse = 0, rightExpanse = 0, TotalExpanse = 0;

//void calculateExpanse(data* h, int LeftOrRight){
//    if (LeftOrRight == -1){
//        //NOP
//    }
//    if (LeftOrRight == LEFT){
//        if (h->left != NULL){
//            leftExpanse++;
//            calculateExpanse(h->left, LEFT);
//        }
//    }
//    if (LeftOrRight == RIGHT){
//        if (h->right != NULL){
//            rightExpanse++;
//            calculateExpanse(h->right, RIGHT);
//        }
//    } 
//}

int DrawOnScreen(data* _head, int _levels) {
    
    head = _head;
    levels = _levels;
//    calculateExpanse(head, LEFT);
//    calculateExpanse(head, RIGHT);
    
//    TotalExpanse = leftExpanse + rightExpanse + 1;
    
//    PixelsPerLevel = 150;
//    ScreenSize.x = levels*Block_HL;
    ScreenSize.x = pow(2, levels)*circleRadius;
    
    ScreenSize.y = ScreenSize.x/VERTICAL_FACTOR;//(PixelsPerLevel + 2*circleRadius)*levels;
    PixelsPerLevel = ScreenSize.y/(levels);
    Parent.x = ScreenSize.x/2;
    Parent.y = ScreenSize.y - circleRadius;
    init_graphics("Branch And Bound", WHITE);

    set_visible_world(0, 0, ScreenSize.x, ScreenSize.y);

    event_loop(NULL, NULL, NULL, drawscreen);

    close_graphics();
    cout<<"END";
    return 0;
}


void drawTree(data* node, int levelAt, int leftOrRight, Point ParentNodeCoords, Point ParentNodeCircleCenterCoords){
    //draw this node
    Point NodeCoords(0, 0);
    Point NodeCircleCenterCoords(0, 0);
    if (leftOrRight == -1){
        //head node
        NodeCoords = ParentNodeCoords;
        NodeCircleCenterCoords = NodeCoords;
    }
    else if (leftOrRight == LEFT){
        NodeCoords.x = ParentNodeCoords.x - pow(2, levels - levelAt - 2)*circleRadius;
        NodeCoords.y = ParentNodeCoords.y - PixelsPerLevel - 2*circleRadius;
        NodeCircleCenterCoords.x = NodeCoords.x;// + circleRadius;
        NodeCircleCenterCoords.y = NodeCoords.y;
    }
    else if (leftOrRight == RIGHT){
        NodeCoords.x = ParentNodeCoords.x + pow(2, levels - levelAt - 2)*circleRadius;
        NodeCoords.y = ParentNodeCoords.y - PixelsPerLevel - 2*circleRadius;
        NodeCircleCenterCoords.x = NodeCoords.x;// - circleRadius;
        NodeCircleCenterCoords.y = NodeCoords.y;

    }
    
    char* text = new char[10];
//    sprintf(text, "%d", NodeCoords.x);
    sprintf(text, "%d", node->blocknum);
    drawCircle(NodeCircleCenterCoords, circleRadius);
    drawtext(NodeCircleCenterCoords.x, NodeCircleCenterCoords.y, text, FLT_MAX, FLT_MAX);
    delete[] text;
    
    //draw the lines
//    if (leftOrRight == LEFT){
//        //there is a parent for this 
//        drawline(NodeCircleCenterCoords.x, NodeCircleCenterCoords.y, ParentNodeCircleCenterCoords.x , ParentNodeCircleCenterCoords.y);
//    }
//    else if (leftOrRight == RIGHT){
//        //there is a parent for this 
//        drawline(NodeCircleCenterCoords.x, NodeCircleCenterCoords.y, ParentNodeCircleCenterCoords.x , ParentNodeCircleCenterCoords.y);
//    }
    
    if (leftOrRight != -1){
        drawline(NodeCircleCenterCoords.x, NodeCircleCenterCoords.y, ParentNodeCircleCenterCoords.x , ParentNodeCircleCenterCoords.y);
        //drawline(NodeCircleCenterCoords.x, NodeCircleCenterCoords.y, ParentNodeCoords.x , ParentNodeCoords.y);
    }
    
    
    if (node->left != NULL){
        drawTree(node->left, levelAt+1, LEFT, NodeCoords, NodeCircleCenterCoords);
    }
    if (node->right != NULL){
        drawTree(node->right, levelAt+1, RIGHT, NodeCoords, NodeCircleCenterCoords);
    }
    
}

void drawscreen() {
//    we have to draw, binary tree of levels given by variable 'levels'
//    we start with x pixel distance between two child nodes, and reduce this by circleRadius in every level
//    by the time we complete the levels, we lose x - levels*circleRadius
//    if this is set to circleRadius*2 pixels, then
//    x = (2+levels)*circleRadius;
    
    clearscreen();
    setfontsize(8);
    initDistanceBetweenNodes = (2+levels)*circleRadius;
    
    drawTree(head, 0,-1, Parent, ParentCircle);
}
