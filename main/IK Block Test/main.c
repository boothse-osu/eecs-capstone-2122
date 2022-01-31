//Test program for the inverse kinematics code

#include <assert.h>
#include "../Mobile3DPrinter/printer_struct.h"
#include "../Mobile3DPrinter/kinematics.h"

//Generates a motor, converting degrees to radians to make things easier on my poor brain
struct Motor generate_motor_deg(float min, float max) {

	struct Motor mtr;

	mtr.angle = 0.f;
	mtr.min_angle = (float)(min * M_PI/180);
	mtr.max_angle = (float)(max * M_PI/18);

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

	struct Link links[6];
	
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
	glm_translate(tmp, (vec3) { 0.f, 0.f, 0.f });

	links[3] = generate_link(tmp, 0, (vec3) {0.f,0.f,1.f},1.f);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) {0.f,2.f,0.f});

	links[4] = generate_link(tmp, 0, (vec3) { 1.f, 0.f, 0.f }, 1.f);

	glm_mat4_copy(id, tmp);
	glm_translate(tmp, (vec3) { 0.f, 1.f, 0.f });

	links[5] = generate_link(tmp, 1, (vec3) { 1.f, 0.f, 0.f }, 1.f);

	//Create the printer

	//Cursed, but nothing else was working...
	struct Printer prn = { {links[0],links[1],links[2],links[3],links[4],links[5]},{motors[0],motors[1],motors[2],motors[3],motors[4]} };

	return prn;
}

//Performs a test case
void test_case(struct Printer* prn, vec3 target, vec3 normal) {

	glm_vec3_normalize(normal);

	printf("	Starting test case for target <%f,%f,%f> and normal <%f,%f,%f>\n", target[0], target[1], target[2], normal[0], normal[1], normal[2]);

	//The only thing that would be changed
	mat4 link_matrices[5];

	for (int i = 0; i < 5; i++) {
		glm_mat4_copy(prn->links[i].link_mat, link_matrices[i]);
	}

	forward_kinematics(prn);
	inverse_kinematics(prn, target, normal);
	
	vec3 target_results;
	vec3 norm_results;
	printer_get_tip(prn, target_results);
	printer_get_normal(prn, norm_results);

	//print_vec3(target_results);
	//print_vec3(norm_results);
	
	for (int i = 0; i < 3; i++) {
		assert(target_results[i] == target[i]);
	}
	printf("		XYZ target <%f,%f,%f>: \x1B[32mPASSED\033[0m\n",target[0],target[1],target[2]);

	float margin = 0.001;
	for (int i = 0; i < 3; i++) {
		//printf("I: %i\n",i);
		assert(norm_results[i] <= normal[i]+margin && norm_results[i] >= normal[i]-margin);
	}
	printf("		Normal target <%f,%f,%f>: \x1B[32mPASSED\033[0m\n",normal[0],normal[1],normal[2]);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				assert(link_matrices[i][j][k] == prn->links[i].link_mat[j][k]);
			}
		}
	}
	printf("		Verified structure has not been changed: \x1B[32mPASSED\033[0m\n");

	printf("		Test case: \x1B[32mPASSED\033[0m\n");
}

int main() {

	struct Printer prn = generate_printer();

	//print_printer(&prn);

	printf("Move just the first motor\n");
	test_case(&prn, (vec3) { 4.f, 4.f, 2.f }, (vec3){0.f, 1.f,0.f});

	printf("Move just the second motor\n");
	test_case(&prn, (vec3) { 4.f, 4.f, 3.f }, (vec3) { 0.f, 1.f, 0.f });

	printf("Move just the third motor\n");
	test_case(&prn, (vec3) { 4.f, 5.f, 2.f }, (vec3) { 0.f, 1.f, 0.f });

	printf("Move in the Z axis\n");
	test_case(&prn, (vec3) { 4.f, 4.f, 2.f }, (vec3) { 1.f, 1.f, 0.f });

	printf("Move in the X axis\n");
	test_case(&prn, (vec3) { 4.f, 4.f, 2.f }, (vec3) { 1.f, 1.f, 1.f });

	printf("Move all at once\n");
	test_case(&prn, (vec3) { 3.f, 3.f, 3.f }, (vec3) { -1.f, -1.f, -1.f });

	return 0;
}