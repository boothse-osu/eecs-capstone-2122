#ifndef PRINTER_KINEMATICS
#define PRINTER_KINEMATICS

#include "printer_struct.h"

//Forward kinematics. Updates the matrices in the printer struct based
//	on the angles of the motor structs in it.
//Precondition: The Printer struct has the angles set to the desired values
void forward_kinematics(struct Printer*);

void inverse_kinematics(struct Printer*, vec3 target, vec3 normal);

#endif