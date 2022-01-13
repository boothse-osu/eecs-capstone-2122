#ifndef PRINTER_STRUCT
#define PRINTER_STRUCT

#include <cglm.h>

//Motor struct
//Contains current angle, min and max angles
struct Motor {
	double angle;
	double max_angle;
	double min_angle;
};

//Link of the kinematics assembly
//link is a 4x4 matrix showing the translation from the end of the link to the
//	central point - putting 0,0,0,1 in should provide the coords of the end of the link
//Length is technically redundant, but might be good to have for
//	some applications
struct Link{
	double length;
	mat4 link;
};

//Printer struct. For the printer as a whole.
struct Printer{
	struct Link links[5];
	struct Motor motors[5];
};

#endif