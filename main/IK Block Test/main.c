//Test program for the inverse kinematics code

#include <assert.h>
#include "../Mobile3DPrinter/printer_struct.h"
#include "../Mobile3DPrinter/kinematics.h"

int main() {

	struct Printer prn = generate_printer();

	//print_printer(&prn);

	printf("Move just the first motor\n");
	inverse_kinematics(&prn, (vec3) { 2.f, 3.f, 1.f }, (vec3){0.f, 1.f,0.f});

	//printf("Move just the second motor\n");
	//inverse_kinematics(&prn, (vec3) { 2.f, 3.f, 2.f }, (vec3) { 0.f, 1.f, 0.f });

	//printf("Move just the third motor\n");
	//inverse_kinematics(&prn, (vec3) { 2.f, 4.f, 2.f }, (vec3) { 0.f, 1.f, 0.f });

	//printf("Move in the Z axis\n");
	//inverse_kinematics(&prn, (vec3) { 2.f, 4.f, 2.f }, (vec3) { 1.f, 1.f, 0.f });

	//printf("Move in the X axis\n");
	//inverse_kinematics(&prn, (vec3) { 2.f, 4.f, 2.f }, (vec3) { 1.f, 1.f, 1.f });

	//printf("Move all at once\n");
	//inverse_kinematics(&prn, (vec3) { 4.f, 2.f, 4.f }, (vec3) { -1.f, -1.f, -1.f });

	return 0;
}