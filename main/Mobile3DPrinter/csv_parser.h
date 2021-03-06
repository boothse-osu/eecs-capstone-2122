#ifndef CSV_PARSER
#define CSV_PARSER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Buffer size for number of chars in lines for csv parser
//Set to 2x what we should need
#define CSVBUFFSIZE 256

//Struct for points on the printing path
struct Point {
	float x;
	float y;
	float z;
	float theta;
	float phi;
	int extrusion;
	float distance;
};

//Struct for the whole path
struct Path {
	//Holds raw points in terms of 3D space
	struct Point* raw_points;
	//Holds points which have gone through IK in terms of motor angles relative to the last move
	struct Point* points;
	int size;
	int cap;
};

//Dynamic memory handling for adding points to the printing path
//Returns current size of the printing path
int csv_parse(struct Path*, char*);

#endif
