#ifndef PRINTER_STRUCT
#define PRINTER_STRUCT

#include <cglm.h>
#include "printer_defines.h"

//Motor struct
//Contains current angle, min and max angles in radians
struct Motor {
	float angle;
	float max_angle;
	float min_angle;
};

//Link of the kinematics assembly
struct Link{
	//link mat is a 4x4 matrix showing the translation from the end of the link to the
	//	central point - putting 0,0,0,1 in should provide the coords of the end of the link
	//  should never be changed during the operation of the printer. Should probably be const.
	mat4 link_mat;
	//Easier way to get vector
	vec3 home;
	//The current location of the end of the link compared to the base of the link. Prismatic joints "grow".
	mat4 current_mat;
	//absolute mat is a 4x4 matrix showing the translation from the base of the very first link to the end of this link.
	//	once everything is all said and done putting 0,0,0,1 through the last link should provide the coords of the end effector
	mat4 absolute_mat;
	//Whether the joint before the link is prismatic or not. If 0 it's rotational. If not, it is prismatic.
	int prismatic;
	//The axis the printer moves on if prismatic, or the one it moves around if rotational.
	//MUST be a normal vector or the movement ratios will be messed up
	vec3 axis;
	//The ratio of motor angle to movement. Should be 1 for rotational, may depend for prismatic.
	float move_ratio;
};

//Printer struct. For the printer as a whole.
struct Printer{
	struct Link links[NUM_LINKS];
	struct Motor motors[NUM_MOTORS];
};

//Gets vector3 position of the printer's end effector, assuming that the final absolute link matrix is up to date
void printer_get_tip(struct Printer*, vec3);

//Gets the normal vector relative to the surface of the last two joints.
void printer_get_normal(struct Printer*, vec3);

//Printer constructor helpers
struct Printer generate_printer();

//Debug helpers
void print_vec3(vec3);

void print_vec3_sphere_coords(vec3);

void print_mat4(mat4);

void print_link(struct Link*);

void print_motor(struct Motor*);

void print_printer(struct Printer*);

void print_link_position(struct Link*);

void print_printer_link_positions(struct Printer*);

#endif