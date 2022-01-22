//Test program for the inverse kinematics code

#include "../Mobile3DPrinter/printer_struct.h"

struct Motor generate_motor(float min, float max) {

	struct Motor mtr;

	mtr.angle = 0.f;
	mtr.min_angle = min;
	mtr.max_angle = max;

	return mtr;
}

struct Link generate_link(mat4 link, int prismatic, vec3 axis, float ratio) {

	struct Link lnk;
	
	//Set the link matrix
	glm_mat4_copy(link,lnk.link_mat);
	
	//Set up the other matrices
	mat4 id = GLM_MAT4_IDENTITY_INIT;
	glm_mat4_copy(id, lnk.current_mat);
	glm_mat4_copy(id,lnk.absolute_mat);

	lnk.prismatic = prismatic;
	
	glm_vec3_copy(axis,lnk.axis);

	lnk.move_ratio = ratio;

	return lnk;
}

//Changes the prn to be a valid printer, defined in the code
//TODO: Make this take in variables and just generally be less awful
struct Printer generate_printer() {

	//Generate the motors
	struct Motor motors[5];
	//90 degrees for the test case
	for (int i = 0; i < 5; i++) {
		motors[i] = generate_motor(-90.f, 90.f);
	}

	//Generate the links
	//For the test program
	//Initial link lengths of 3,2,1,2,1
	//Not all 1 to create enough variation to catch certain errors
	//First 3 prismatic, last two rotational
	//Moves along X,Z,Y axes. X and Y might be swapped in final printer
	//Rotational are around Z, then X axis.
	//Need to talk to MEs
	//Movement ratios of 0.5, 0.25, 0.25 for the prismatic joints just be be sure it works
	// (One degree of rotmovement = 0.5 inches of lateral movement. Scale is a mess.)

	struct Link links[5];
	
	mat4 id = GLM_MAT4_IDENTITY_INIT;
	mat4 tmp = GLM_MAT4_IDENTITY_INIT;

	glm_translate(tmp, (vec3) { 3.f, 0.f, 0.f });

	links[0] = generate_link(tmp, 1, (vec3){1.f,0.f,0.f}, 0.5);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) { 0.f, 0.f, 2.f });

	links[1] = generate_link(tmp, 1, (vec3) {0.f,0.f,1.f},0.25);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) { 0.f, 1.f, 0.f });

	links[2] = generate_link(tmp, 1, (vec3) { 0.f, 1.f, 0.f }, 0.25);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) { 0.f, 2.f, 0.f });

	links[3] = generate_link(tmp, 0, (vec3) {0.f,0.f,1.f},1.f);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) {0.f,1.f,0.f});

	links[4] = generate_link(tmp, 0, (vec3) { 1.f, 0.f, 0.f }, 1.f);

	//Create the printer

	struct Printer prn = { .motors = *motors, .links = *links };

	return prn;
}

/*
void test_forward_kinematics(struct Printer*) {

}
*/

int main() {

	struct Printer prn = generate_printer();

	return 0;
}