//Test program for the inverse kinematics code

#include <assert.h>
#include <math.h>
#include "../Mobile3DPrinter/printer_struct.h"
#include "../Mobile3DPrinter/kinematics.h"

float deg2rad(float deg) {

	return deg * (M_PI / 180.f);
}

int main() {

	struct Printer prn = generate_printer();

	forward_kinematics(&prn);

	//print_printer(&prn);
	vec3 home_position;
	printer_get_tip(&prn,home_position);

	vec2 home_normal;
	printer_get_normal(&prn, home_normal);

	printf("Home normal:\n");
	printf("<%f, %f>\n", home_normal[0], home_normal[1]);
	printf("Home position:\n");
	print_vec3(home_position);

	//printf("Home structure:\n");
	//print_printer(&prn);

	printf("Move just the first motor\n");
	if(ik_test_case(&prn, (vec3) { 2.f, 3.f, 1.f }, deg2rad(90.f), deg2rad(0.f))) return;

	printf("Move just the second motor\n");
	if(ik_test_case(&prn, (vec3) { 2.f, 3.f, 2.f }, deg2rad(0.f), deg2rad(90.f))) return;

	printf("Move just the third motor\n");
	if (ik_test_case(&prn, (vec3) { 2.f, 4.f, 2.f }, deg2rad(45.f), deg2rad(45.f))) return;

	printf("Move just theta\n");
	if (ik_test_case(&prn, (vec3) { 2.f, 4.f, 2.f }, deg2rad(45.f), deg2rad(90.f))) return;

	printf("Move just phi\n");
	if (ik_test_case(&prn, (vec3) { 2.f, 4.f, 2.f }, deg2rad(90.f), deg2rad(45.f))) return;

	printf("Move all\n");
	if (ik_test_case(&prn, (vec3) { 3.f, 5.f, 3.f }, deg2rad(15.f), deg2rad(25.f))) return;

	return 0;
}