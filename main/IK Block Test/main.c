//Test program for the inverse kinematics code

#include <assert.h>
#include "../Mobile3DPrinter/printer_struct.h"
#include "../Mobile3DPrinter/kinematics.h"

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