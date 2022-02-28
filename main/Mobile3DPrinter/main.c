#include <stdio.h>
#include <string.h>
#include "printer_struct.h"
#include "kinematics.h"
#include "csv_parser.h"

//Initial buffer size for printing path (number of points)
#define PATH_SIZE 32

//Convert sphere coordinates into a normal vector to feed into the IK algorithms
//Probably should have had that take in sphere coords to start with...
void sphere_to_normal(vec3 norm, float theta, float phi) {

	norm[0] = (float)(cos(theta) * sin(phi));
	norm[1] = (float)(sin(theta) * sin(phi));
	norm[2] = (float)(cos(theta));

	glm_vec3_normalize(norm);
}

int main(int argc,char* argv[]) {

	//Get the CSV file path from command line arguments

	/*
	//Check to ensure there is a filepath
	if (argc < 2) {
		printf("ERROR: No filepath provided\n");
		return 0
	}

	char* filepath = argv[1];
	printf("Printing %s",filepath);
	*/

	char* filepath = "D:\\School\\Capstone\\Capstone Code\\eecs-capstone-2122\\main\\Tool Paths.csv";

	//Parse the CSV file
	struct Path print_path;
	print_path.points = (struct Point*) malloc(sizeof(struct Point)*PATH_SIZE); //Say your prayers boys we doing dynamic memory
	print_path.size = 0;
	print_path.cap = PATH_SIZE;

	int parse_fail = csv_parse(&print_path,filepath);
	if (parse_fail) {
		free(print_path.points);
		printf("Unexpected error: File parsing failed\n");
		return 0;
	}

	printf("Loaded file: %s\n",filepath);

	//Setup
	
	//Home the printer
	//printer_home()

	//Create a new printer struct and hard set the angles to zero
	struct Printer printer;
	
	//Will eventually make use of defines for constants
	//Has all motor angles set to zero by default
	generate_printer();

	int extruding = 0;

	//While there are still points to handle, handle each point
	for (int i = 0; i < print_path.size; i++) {

		struct Point pnt = print_path.points[i];

		vec3 target = { pnt.x,pnt.y,pnt.z };
		vec3 normal;
		sphere_to_normal(normal, pnt.theta, pnt.phi);
		inverse_kinematics(&printer, target, normal);

		//Extrusion
		if (pnt.extrusion) {
			//Begin extrusion
			extruding = 1;
		}
		else {
			//End extrusion
			extruding = 0;
		}

		//Motor handling I guess?

	}

	//Shutdown
	if (extruding) {
		//End extrusion
		extruding = 0;
	}

	free(print_path.points);

	return 0;
}