#ifndef PRINTER_KINEMATICS
#define PRINTER_KINEMATICS

#include "printer_struct.h"
#include <assert.h>

//Forward kinematics. Updates the matrices in the printer struct based
//	on the angles of the motor structs in it.
//Precondition: The Printer struct has the angles set to the desired values
void forward_kinematics(struct Printer*);

int inverse_kinematics(struct Printer*, vec3 target, vec2 normal, vec3 target_delta, vec2 normal_delta);

int ik_test_case(struct Printer*,vec3 target, vec2 normal);

#endif