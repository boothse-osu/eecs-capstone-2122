//Test program for the inverse kinematics code

#include "../Mobile3DPrinter/printer_struct.h"
#include "../Mobile3DPrinter/kinematics.h"

//Generates a motor, converting degrees to radians to make things easier on my poor brain
struct Motor generate_motor_deg(float min, float max) {

	struct Motor mtr;

	mtr.angle = 0.f;
	mtr.min_angle = min * M_PI/180;
	mtr.max_angle = max * M_PI/18;

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
		struct Motor m = generate_motor_deg(-90.f,90.f);
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

	struct Link links[5];
	
	mat4 id = GLM_MAT4_IDENTITY_INIT;
	mat4 tmp = GLM_MAT4_IDENTITY_INIT;

	glm_translate(tmp, (vec3) { 3.f, 0.f, 0.f });

	links[0] = generate_link(tmp, 1, (vec3){1.f,0.f,0.f}, 1.f);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) { 0.f, 0.f, 2.f });

	links[1] = generate_link(tmp, 1, (vec3) {0.f,0.f,1.f},1.f);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) { 0.f, 1.f, 0.f });

	links[2] = generate_link(tmp, 1, (vec3) { 0.f, 1.f, 0.f }, 1.f);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) { 0.f, 2.f, 0.f });

	links[3] = generate_link(tmp, 0, (vec3) {0.f,0.f,1.f},1.f);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) {0.f,1.f,0.f});

	links[4] = generate_link(tmp, 0, (vec3) { 1.f, 0.f, 0.f }, 1.f);

	//Create the printer

	//Cursed, but nothing else was working...
	struct Printer prn = { {links[0],links[1],links[2],links[3],links[4]},{motors[0],motors[1],motors[2],motors[3],motors[4]} };

	return prn;
}

/*
void test_forward_kinematics(struct Printer*) {

}
*/

int main() {

	struct Printer prn = generate_printer();

	//print_printer(&prn);

	forward_kinematics(&prn);

	vec3 target_results;
	vec3 norm_results;
	printer_get_tip(&prn,target_results);
	printer_get_normal(&prn, norm_results);

	print_vec3(target_results);
	print_vec3(norm_results);

	vec3 target = { 2.f,2.f,2.f };
	vec3 normal = { -1.f,-1.f,0.f };
	glm_vec3_normalize(normal);
	inverse_kinematics(&prn, target, normal);

	printer_get_tip(&prn, target_results);
	print_vec3(target_results);
	printer_get_normal(&prn, norm_results);
	print_vec3(norm_results);
	print_printer_link_positions(&prn);

	return 0;
}