//Test program for the inverse kinematics code

#include <assert.h>
#include "../Mobile3DPrinter/printer_struct.h"
#include "../Mobile3DPrinter/kinematics.h"

int main() {

	struct Printer prn = generate_printer();

	//print_printer(&prn);

	printf("Move just the first motor\n");
	if(ik_test_case(&prn, (vec3) { 2.f, 3.f, 1.f }, (vec3){0.f, 1.f,0.f})) return;

	printf("Move just the second motor\n");
	if(ik_test_case(&prn, (vec3) { 2.f, 3.f, 2.f }, (vec3) { 0.f, 1.f, 0.f })) return;

	printf("Move just the third motor\n");
	if (ik_test_case(&prn, (vec3) { 2.f, 4.f, 2.f }, (vec3) { 0.f, 1.f, 0.f })) return;

	printf("Move around Z less than 90 degrees\n");
	if (ik_test_case(&prn, (vec3) { 2.f, 4.f, 2.f }, (vec3) { 1.f, 1.f, 0.f })) return;

	printf("Move around Z to 90 degrees\n");
	if (ik_test_case(&prn, (vec3) { 2.f, 4.f, 2.f }, (vec3) { 1.f, 0.f, 0.f })) return;

	printf("Move around Z to more than 90 degrees\n");
	if (ik_test_case(&prn, (vec3) { 2.f, 4.f, 2.f }, (vec3) { 1.f, -1.f, 0.f })) return;

	return 0;
}