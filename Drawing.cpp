#include "Drawing.h"

#include <iostream>
#include <cfloat>

using namespace::std;

#define LEFT 1
#define RIGHT 2

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
int PixelsPerLevel = 0;
int initDistanceBetweenNodes = 0;

int DrawOnScreen(data* _head, int _levels) {
    
    head = _head;
    levels = _levels;
    PixelsPerLevel = 150;
    ScreenSize.y = (PixelsPerLevel + 2*circleRadius)*levels;
    Parent.x = ScreenSize.x/2;
    Parent.y = ScreenSize.y - circleRadius;
    init_graphics("Branch And Bound", WHITE);

    set_visible_world(0, 0, ScreenSize.x, ScreenSize.y);

    event_loop(NULL, NULL, NULL, drawscreen);

    close_graphics();
    cout<<"END";
    return 0;
}


void drawTree(data* node, int levelAt, int leftOrRight, Point PointAt){
    //draw this node
    Point NodeCoords(0, 0);
    if (leftOrRight == -1){
        //head node
        NodeCoords = PointAt;
    }
    else if (leftOrRight == LEFT){
        NodeCoords.x = PointAt.x - (circleRadius*2);
        NodeCoords.y = PointAt.y - PixelsPerLevel - 2*circleRadius;
    }
    else if (leftOrRight == RIGHT){
        NodeCoords.x = PointAt.x + (circleRadius*2);
        NodeCoords.y = PointAt.y - PixelsPerLevel - 2*circleRadius;
    }
    
    char* text = new char[10];
//    sprintf(text, "%d", NodeCoords.x);
    sprintf(text, "%d", node->blocknum);
    drawCircle(NodeCoords, circleRadius);
    drawtext(NodeCoords.x, NodeCoords.y, text, FLT_MAX, FLT_MAX);
    delete[] text;
    
    if (node->left != NULL){
        drawTree(node->left, levelAt+1, LEFT, NodeCoords);
    }
    if (node->right != NULL){
        drawTree(node->right, levelAt+1, RIGHT, NodeCoords);
    }
    
}

void drawscreen() {
//    we have to draw, binary tree of levels given by variable 'levels'
//    we start with x pixel distance between two child nodes, and reduce this by circleRadius in every level
//    by the time we complete the levels, we lose x - levels*circleRadius
//    if this is set to circleRadius*2 pixels, then
//    x = (2+levels)*circleRadius;
    
    clearscreen();
    setfontsize(5);
    initDistanceBetweenNodes = (2+levels)*circleRadius;
    
    drawTree(head, 0,-1, Parent);
}
