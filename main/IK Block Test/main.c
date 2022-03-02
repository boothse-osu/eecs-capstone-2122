//Test program for the inverse kinematics code

#include <assert.h>
#include "../Mobile3DPrinter/printer_struct.h"
#include "../Mobile3DPrinter/kinematics.h"

int main() {

	struct Printer prn = generate_printer();

	//print_printer(&prn);

	printf("Move just the first motor\n");
	ik_test_case(&prn, (vec3) { 4.f, 4.f, 2.f }, (vec3){0.f, 1.f,0.f});

	printf("Move just the second motor\n");
	ik_test_case(&prn, (vec3) { 4.f, 4.f, 3.f }, (vec3) { 0.f, 1.f, 0.f });

	printf("Move just the third motor\n");
	ik_test_case(&prn, (vec3) { 4.f, 5.f, 2.f }, (vec3) { 0.f, 1.f, 0.f });

	printf("Move in the Z axis\n");
	ik_test_case(&prn, (vec3) { 4.f, 4.f, 2.f }, (vec3) { 1.f, 1.f, 0.f });

	printf("Move in the X axis\n");
	ik_test_case(&prn, (vec3) { 4.f, 4.f, 2.f }, (vec3) { 1.f, 1.f, 1.f });

	printf("Move all at once\n");
	ik_test_case(&prn, (vec3) { 3.f, 3.f, 3.f }, (vec3) { -1.f, -1.f, -1.f });

	return 0;
}