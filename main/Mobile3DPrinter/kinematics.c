#include "kinematics.h"

//Forward kinematics. Updates the matrices in the printer struct based
//	on the angles of the motor structs in it.
//Precondition: The Printer struct has the angles set to the desired values
//Does not check for motor angles, the IK algorithm will do that
void forward_kinematics(struct Printer* prn)
{
	//Only 5 links in the kinematic chain. Would have left this as a define, but my IK won't work
	//	for an arbitary configuration anyways. If I could, I would.

	//Go through the motors and links and update the link matrices based on the angles.
	for (int i = 0; i < 6; i++) {

		if (prn->links[i].prismatic) {
			
			//If a prismatic joint, translate the link matrix by the scaling factor times the movement axis.
			
			//Generate the vector to translate by
			vec3 move_vec;
			glm_vec3_scale(prn->links[i].axis,prn->links[i].move_ratio * prn->motors[i].angle,move_vec);
			
			//Copy the base link value to the current value
			glm_mat4_copy(prn->links[i].link_mat, prn->links[i].current_mat);
			
			//Translate the current value
			glm_translate(prn->links[i].current_mat, move_vec);
		}
		else {
			//If not a prismatic joint, rotate the link matrix around the movement axis by the angle times the scaling factor
			//	(in case of gear ratio stuff).

			//Copy the base link value to the current value
			glm_mat4_copy(prn->links[i].link_mat, prn->links[i].current_mat);

			//Rotate it
			glm_rotate(prn->links[i].current_mat, prn->motors[i].angle * prn->links[i].move_ratio, prn->links[i].axis);

		}
	}

	//Now, take all the relative values in the links and update their absolute matrices
	for (int i = 0; i < 6; i++) {

		mat4 prev_matrix;

		//Special case for the first matrix; use the identity matrix as previous matrix
		//Previous matrix is the previous absolute matrix
		if (i == 0) {
			glm_mat4_identity(prev_matrix);
		}
		else {
			glm_mat4_copy(prn->links[i - 1].absolute_mat, prev_matrix);
		}

		glm_mat4_mul(prev_matrix, prn->links[i].current_mat,prn->links[i].absolute_mat);

	}
}

//Inverse kinematics function
//Warning: Not at all portable. Basically designed with a lot of assumptions.
void inverse_kinematics(struct Printer* prn, vec3 target, vec3 normal) {

	
	//Handle rotational joints. May need to redo this whole section later.

	//Calculate axis coordinates
	
	//r1 = arctan(x/y)
	float r1 = (float)atan2(-1.f*(double)normal[0], normal[1]);
	//r2 = arctan()
	float r2 = (float)atan2(normal[2], sqrt(((double)normal[0]*(double)normal[0])+((double)normal[1]*(double)normal[1])));

	//Set the rotational joints to those coordinates if possible
	//Making assumptions about these last two motors being Z axis rotation and X axis rotation
	//	specifically
	prn->motors[3].angle = r1;
	//printf("Z-Axis: %f\n",r1);

	prn->motors[4].angle = r2;
	//printf("X-Axis: %f\n",r2);

	//Do FK on the last two joints to get the final two links as a single vector
	forward_kinematics(prn);

	//DEBUG
	/*
	printf("Position\n");
	vec3 results;
	printer_get_tip(prn, results);
	print_vec3(results);
	printf("Normal\n");
	printer_get_normal(prn, results);
	print_vec3(results);
	//print_printer_link_positions(prn);
	*/
	//END

	//Move the prismatic joints to get it to where it needs to go
	vec3 end_effector;
	printer_get_tip(prn, end_effector);

	//Move the pristmatic joints one by one
	vec3 differences = { target[0] - end_effector[0],target[1] - end_effector[1],target[2] - end_effector[2] };
	//printf("Differences: ");
	//print_vec3(differences);

	//Again making assumptions based on the design of our specific printer
	//X-axis
	prn->motors[0].angle += differences[0] / prn->links[0].move_ratio;

	//Z-axis
	prn->motors[1].angle += differences[2] / prn->links[1].move_ratio;

	//Y-axis
	prn->motors[2].angle += differences[1] / prn->links[2].move_ratio;

	//Do FK to ensure that the end of the printer is in the right place
	forward_kinematics(prn);

}

//Performs a test case
void ik_test_case(struct Printer* prn, vec3 target, vec3 normal) {

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
	printf("		XYZ target <%f,%f,%f>: \x1B[32mPASSED\033[0m\n", target[0], target[1], target[2]);

	float margin = 0.001;
	for (int i = 0; i < 3; i++) {
		//printf("I: %i\n",i);
		assert(norm_results[i] <= normal[i] + margin && norm_results[i] >= normal[i] - margin);
	}
	printf("		Normal target <%f,%f,%f>: \x1B[32mPASSED\033[0m\n", normal[0], normal[1], normal[2]);

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
