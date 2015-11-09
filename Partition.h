/* 
 * File:   Partition.h
 * Author: parul
 *
 * Created on November 2, 2015, 7:38 PM
 */

#ifndef PARTITION_H
#define	PARTITION_H
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "BandB.h"
void sorting(vector<block>, int**);
void InitialSol(Net*, int**, int**, int, int, int*);
int lowerBound_initial(Net*, int*, int*, int, int);
void sort_netnums(Net* nets, int** sort, int numnet);
data* makeTree(vector<block> Blocks, int index, data* Parent, int LeftOrRightChild, int* order, int numOfBlocks, int* lb_best, int** left, int** right, int* count_node, int numNets);
int calculateCO(data* node, vector<block> Blocks, int numNets);
void Initial_solution_swap(Net* nets, int** left, int** right, int numOfBlocks, vector<block> Blocks);
#endif	/* PARTITION_H */

