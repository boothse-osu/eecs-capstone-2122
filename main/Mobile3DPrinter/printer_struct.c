
#include "printer_struct.h"

//Gets the position of the end of a link
void get_link_position(struct Link* lnk, vec3 out) {

	vec3 new = GLM_VEC3_ZERO_INIT;
	glm_mat4_mulv3(lnk->absolute_mat, new, 1.f, out);
}

//Gets position of the end effector
void printer_get_tip(struct Printer* prn, vec3 out) {

	get_link_position(&(prn->links[4]), out);
}

//Normal vector from the print surface
void printer_get_normal(struct Printer* prn, vec3 out) {

	//Get the position of link 3 minus the position of link 4
	vec3 lnk5;
	get_link_position(&(prn->links[5]), lnk5);
	vec3 lnk4;
	get_link_position(&(prn->links[4]), lnk4);

	vec3 norm;
	glm_vec3_sub(lnk4, lnk5, norm);

	//Reverse it
	glm_vec3_scale(norm, -1.f, norm);

	//Normalize
	glm_vec3_normalize_to(norm,out);
}

//Generates a motor, taking in min and max angles in radians
struct Motor generate_motor(float min, float max) {

	struct Motor mtr;

	mtr.angle = 0.f;
	mtr.min_angle = min;
	mtr.max_angle = max;

	return mtr;
}

//Generates a prismatic link
struct Link generate_link(mat4 link, int prismatic, vec3 axis, float ratio) {

	struct Link lnk;

	//Set the link matrix
	glm_mat4_copy(link, lnk.link_mat);

	//Set up the other matrices
	mat4 id = GLM_MAT4_IDENTITY_INIT;
	glm_mat4_copy(id, lnk.current_mat);
	glm_mat4_copy(id, lnk.absolute_mat);

	lnk.prismatic = prismatic;

	glm_vec3_copy(axis, lnk.axis);

	lnk.move_ratio = ratio;

	return lnk;
}

//Changes the prn to be a valid printer, defined in the code
//TODO: Make this take in variables and just generally be less awful
struct Printer generate_printer() {

	//Generate the motors
	struct Motor motors[NUM_MOTORS];

	float max_list[NUM_MOTORS] = MOTOR_MAX_LIST;
	float min_list[NUM_MOTORS] = MOTOR_MIN_LIST;

	//90 degrees for the test case
	for (int i = 0; i < NUM_MOTORS; i++) {
		struct Motor m = generate_motor(min_list[i], max_list[i]);
		//print_motor(&m);
		motors[i] = m;
	}

	//Generate the links
	//For the test program
	//Initial link lengths of 3,2,1,2,1
	//Not all 1 to create enough variation to catch certain errors
	//First 3 prismatic, last two rotational
	//Moves along X,Z,Y axes. X and Y might be swapped in final printer
	//Rotational are around Z, then X axis.
	//Need to talk to MEs
	//Movement ratios of 1.f for now just so its easier to debug
	// (One radian of rotmovement = 1 inch of lateral movement. Scale is a mess.)

	struct Link links[NUM_LINKS];

	//Once the IK bug is squashed, this can be refactored to look like the motor code
	mat4 tmp = GLM_MAT4_IDENTITY_INIT;
	glm_translate(tmp, LINK0_HOME);
	
	links[0] = generate_link(tmp, LINK0_PRISMATIC, LINK0_AXIS, LINK0_RATIO);

	glm_mat4_copy(GLM_MAT4_IDENTITY, tmp);
	glm_translate(tmp, LINK1_HOME);

	links[1] = generate_link(tmp, LINK1_PRISMATIC, LINK1_AXIS, LINK1_RATIO);

	glm_mat4_copy(GLM_MAT4_IDENTITY, tmp);
	glm_translate(tmp, LINK2_HOME);

	links[2] = generate_link(tmp, LINK2_PRISMATIC, LINK2_AXIS, LINK2_RATIO);

	glm_mat4_copy(GLM_MAT4_IDENTITY, tmp);
	glm_translate(tmp, LINK3_HOME);

	links[3] = generate_link(tmp, LINK3_PRISMATIC, LINK3_AXIS, LINK3_RATIO);

	glm_mat4_copy(GLM_MAT4_IDENTITY, tmp);
	glm_translate(tmp, LINK4_HOME);

	links[4] = generate_link(tmp, LINK4_PRISMATIC, LINK4_AXIS, LINK4_RATIO);

	//Create the printer

	//Cursed, but nothing else was working...
	struct Printer prn = { {links[0],links[1],links[2],links[3],links[4],links[5]},{motors[0],motors[1],motors[2],motors[3],motors[4]} };

	return prn;
}

//Helpful utilities/despearate debug functions.

void print_mat4(mat4 mat) {

	for (int i = 0; i < 4; i++) {
		printf("%f, %f, %f, %f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
	}
}

void print_vec3(vec3 vec) {
	printf("<%f,%f,%f>\n", vec[0], vec[1], vec[2]);
}

void print_link_matrices(struct Link* lnk) {
	printf("Matrices:\n");
	printf("	Link:\n");
	print_mat4(lnk->link_mat);
	printf("	Current:\n");
	print_mat4(lnk->current_mat);
	printf("	Absolute:\n");
	print_mat4(lnk->absolute_mat);
}

void print_link(struct Link* lnk) {

	print_link_matrices(lnk);
	printf("Axis:\n");
	print_vec3(lnk->axis);
	printf("Ratio: %f, Prismatic: %i\n", lnk->move_ratio, lnk->prismatic);
}

void print_motor(struct Motor* motor) {

	printf("Current:%f, Min:%f, Max:%f\n",motor->angle,motor->min_angle,motor->max_angle);
}

void print_printer(struct Printer* prn) {
	
	for (int i = 0; i < 5; i++) {
		printf("Link %i ",i);
		print_link(&(prn->links[i]));
	}

	for (int i = 0; i < 5; i++) {
		printf("Motor %i ", i);
		print_motor(&(prn->motors[i]));
	}
}

void print_vec3_sphere_coords(vec3 vec) {

	float theta = (float)atan2((double)vec[0], (double)vec[1]);
	float phi = (float)atan2((double)vec[2], (double)vec[1]);

	printf("Theta: %f,Phi: %f\n",theta,phi);
}

void print_link_position(struct Link* lnk) {

	vec3 new = GLM_VEC3_ZERO_INIT;
	glm_mat4_mulv3(lnk->absolute_mat, new, 1.f, new);
	print_vec3(new);
}

void print_printer_link_positions(struct Printer* prn) {

	for (int i = 0; i < 5; i++) {
		printf("Link %i\n", i);
		print_link_position(&(prn->links[i]));
	}
}