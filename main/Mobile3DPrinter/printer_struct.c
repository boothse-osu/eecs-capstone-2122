
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
	vec3 lnk4;
	get_link_position(&(prn->links[4]), lnk4);
	vec3 lnk3;
	get_link_position(&(prn->links[3]), lnk3);

	vec3 norm;
	glm_vec3_sub(lnk3, lnk4, norm);

	//Normalize
	glm_vec3_normalize_to(norm,out);
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