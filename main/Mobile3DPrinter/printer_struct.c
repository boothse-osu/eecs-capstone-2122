
#include "printer_struct.h"

//Gets the position of the end of a link
void get_link_position(struct Link* lnk, vec3 out) {
	
	//glm_mat4_mulv3(lnk->absolute_mat, lnk->home, 1.f, out);

	//Used to work(ish), kept just in case the above turns out to be broken
	vec3 zero = GLM_VEC3_ZERO_INIT;
	glm_mat4_mulv3(lnk->absolute_mat, zero, 1.f, out);
}

//Gets position of the end effector
void printer_get_tip(struct Printer* prn, vec3 out) {

	get_link_position(&(prn->links[5]), out);
}

//Normal vector from the print surface
void printer_get_normal(struct Printer* prn, vec2 out) {

	//Get the position of the end of link 4 minus the position of link 5
	vec3 lnk5;
	get_link_position(&(prn->links[5]), lnk5);
	vec3 lnk4;
	get_link_position(&(prn->links[4]), lnk4);

	vec3 norm;
	glm_vec3_sub(lnk5, lnk4, norm);

	float theta = (float)atan2((double)norm[1], (double)norm[0]);
	float phi = (float)atan2(norm[2], sqrt(((double)norm[0] * (double)norm[0]) + ((double)norm[1] * (double)norm[1])));

	out[0] = theta;
	out[1] = phi;
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
struct Link generate_link(vec3 start, vec3 end, int prismatic, vec3 axis, float ratio) {

	struct Link lnk;

	//Subtract the start from the end
	vec3 home;
	glm_vec3_sub(end,start,home);
	glm_vec3_copy(home, lnk.home);

	//Set the link matrix
	mat4 tmp = GLM_MAT4_IDENTITY_INIT;
	glm_translate_to(tmp, home, lnk.link_mat);

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
	links[0] = generate_link((vec3) {0.f,0.f,0.f},LINK0_END, LINK0_PRISMATIC, LINK0_AXIS, LINK0_RATIO);

	links[1] = generate_link(LINK0_END, LINK1_END, LINK1_PRISMATIC, LINK1_AXIS, LINK1_RATIO);

	links[2] = generate_link(LINK1_END, LINK2_END, LINK2_PRISMATIC, LINK2_AXIS, LINK2_RATIO);

	links[3] = generate_link(LINK2_END, LINK3_END, LINK3_PRISMATIC, LINK3_AXIS, LINK3_RATIO);

	links[4] = generate_link(LINK3_END, LINK4_END, LINK4_PRISMATIC, LINK4_AXIS, LINK4_RATIO);

	//Dummy link for normal purposes. Has no associated motor.
	links[5] = generate_link(LINK4_END, LINK5_END, 1, (vec3){ 0,0,1.f }, 1.f);

	//Create the printer

	//Cursed, but nothing else was working...
	struct Printer prn = { {links[0],links[1],links[2],links[3],links[4],links[5]},{motors[0],motors[1],motors[2],motors[3],motors[4]} };

	return prn;
}

void round_motors(struct Printer* prn) {

	for (int i = 0; i < NUM_MOTORS; i++) {
		prn->motors[i].angle = floor(prn->motors[i].angle * ROUND_PRECISION) / ROUND_PRECISION;
	}
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
	
	for (int i = 0; i < NUM_LINKS; i++) {
		printf("Link %i ",i);
		print_link(&(prn->links[i]));
	}

	for (int i = 0; i < NUM_MOTORS; i++) {
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