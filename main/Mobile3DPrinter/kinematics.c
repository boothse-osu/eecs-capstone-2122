#include "kinematics.h"

//Helper function to check motor angles
//Returns a zero if the new angle is safe
//Returns a one otherwise
//Note that this is only setting the motor angles of the program's internal understanding of the printer's location
int set_motor_angle(struct Printer* prn, int idx, float angle) {
	
	//printf("Changing angle %i to %f!\n", idx, angle);

	if (ANGLE_BOUNDS) {
		//Ensure it is within bounds, otherwie set the angle
		if (angle < prn->motors[idx].min_angle) {
			return 1;
		}

		if (angle > prn->motors[idx].max_angle) {
			return 1;
		}
	}

	prn->motors[idx].angle = angle;


	return 0;
}

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
	for (int i = 0; i < NUM_LINKS; i++) {

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
//Returns an error value, zero if no errors. One if errors.
int inverse_kinematics(struct Printer* prn, vec3 target, vec2 normal, vec3 position_delta, vec2 normal_delta) {

	printf("Calling inverse kinematics!\n");
	printf("Target: %f, %f, %f\n", target[0], target[1], target[2]);
	//Handle rotational joints. May need to redo this whole section later.
	//Calculate axis coordinates
	
	if (IK_NORMALS) {
		vec2 current_norm;
		printer_get_normal(prn, current_norm);

		//Theta
		//XY plane, Z axis
		float theta_current = current_norm[0];
		float theta_diff = normal[0] - theta_current;
		printf("Current R1: %f Target R1: %f R1 Diff: %f\n", theta_current, normal[0], theta_diff);

		//Phi
		//YZ plane,  
		float phi_current = current_norm[1];
		float phi_diff = normal[1] - phi_current;
		printf("Current R2: %f Target R2: %f R2 Diff: %f\n", phi_current, normal[1], phi_diff);

		//Set the rotational joints to those coordinates if possible
		//Making assumptions about these last two motors being Z axis rotation and X axis rotation
		//	specifically
		if (set_motor_angle(prn, 3, prn->motors[3].angle + theta_diff)) return 1;
		normal_delta[0] = theta_diff;

		if (set_motor_angle(prn, 4, prn->motors[4].angle + phi_diff)) return 1;
		normal_delta[1] = phi_diff;
	}
	else {
		normal_delta[0] = 0.f;
		normal_delta[1] = 0.f;
	}

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
	print_printer_link_positions(prn);
	*/
	//END

	//Move the prismatic joints to get it to where it needs to go
	vec3 end_effector;
	printer_get_tip(prn, end_effector);
	//print_vec3(target);
	//print_vec3(end_effector);

	//Move the pristmatic joints one by one
	vec3 differences = { target[0] - end_effector[0],target[1] - end_effector[1],target[2] - end_effector[2] };
	//printf("Differences: ");
	//print_vec3(differences);

	//Again making assumptions based on the design of our specific printer
	//X-axis
	float x_ang = (differences[0] / prn->links[0].move_ratio);
	if (set_motor_angle(prn, 0, prn->motors[0].angle + x_ang)) return 1;
	position_delta[0] = x_ang;

	//Z-axis
	float z_ang = (differences[2] / prn->links[1].move_ratio);
	if (set_motor_angle(prn, 1, prn->motors[1].angle + z_ang)) return 1;
	position_delta[2] = z_ang;

	//Y-axis
	float y_ang = (differences[1] / prn->links[2].move_ratio);
	if (set_motor_angle(prn, 2, prn->motors[2].angle + y_ang)) return 1;
	position_delta[1] = y_ang;

	//Do FK to ensure that the end of the printer is in the right place
	forward_kinematics(prn);

	//Print end position. (Debug purposes)
	/*
	vec3 end_pos;
	printer_get_tip(prn,end_pos);
	print_vec3(end_pos);
	*/

	round_motors(prn);

	return 0;

}

//Performs a test case
int ik_test_case(struct Printer* prn, vec3 target, vec2 normal) {

	printf("	Starting test case for target <%f,%f,%f> and normal <%f,%f>\n", target[0], target[1], target[2], normal[0], normal[1]);

	//The only thing that would be changed
	mat4 link_matrices[5];

	for (int i = 0; i < 5; i++) {
		glm_mat4_copy(prn->links[i].link_mat, link_matrices[i]);
	}

	//Dummy vars to catch the delta position
	vec3 dummy_pos;
	vec2 dummy_norm;

	forward_kinematics(prn);
	if (inverse_kinematics(prn, target, normal, dummy_pos, dummy_norm)) {

		printf("Motor angles outside bounds\n");
		return 1;
	};

	vec3 target_results;
	vec2 norm_results;
	printer_get_tip(prn, target_results);
	printer_get_normal(prn, norm_results);

	//print_vec3(target_results);
	//print_vec3(norm_results);
	print_printer_link_positions(prn);

	float margin = 0.001f;
	bool target_error = false;
	bool normal_error = false;
	for (int i = 0; i < 3; i++) {
		//printf("Target: %f Results: %f", target[i], target_results[i]);
		if(!(target_results[i] <= target[i] + margin && target_results[i] >= target[i] - margin)) target_error = true;
	}
	if (target_error) {
		printf("		XYZ target <%f,%f,%f>: \x1B[31mFAILED\033[0m\n", target[0], target[1], target[2]);
		printf("		XYZ result: <%f,%f,%f>\n", target_results[0], target_results[1], target_results[2]);
	}
	else {
		printf("		XYZ target <%f,%f,%f>: \x1B[32mPASSED\033[0m\n", target[0], target[1], target[2]);
		printf("		XYZ result: <%f,%f,%f>\n", target_results[0], target_results[1], target_results[2]);
	}

	//Check for normal errors
	if (!(norm_results[0] <= normal[0] + margin && norm_results[0] >= normal[0] - margin)) normal_error = true;
	if (!(norm_results[1] <= normal[1] + margin && norm_results[1] >= normal[1] - margin)) normal_error = true;

	//Handle normal errors
	if (normal_error) {
		printf("		Normal target <%f,%f>: \x1B[31mFAILED\033[0m\n", normal[0], normal[1]);
		printf("		Normal result: <%f,%f>\n", norm_results[0], norm_results[1]);
	}
	else {
		printf("		Normal target <%f,%f>: \x1B[32mPASSED\033[0m\n", normal[0], normal[1]);
		printf("		Normal result: <%f,%f>\n", norm_results[0], norm_results[1]);
	}

	//Ensure the structure of the printer has not somehow warped (Not a real concern, meant for a class check-off)
	bool structure_error = false;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				if(!(link_matrices[i][j][k] == prn->links[i].link_mat[j][k])) structure_error = true;
			}
		}
	}
	if(!structure_error) printf("		Verified structure has not been changed: \x1B[32mPASSED\033[0m\n");

	if(!structure_error && !normal_error && !target_error) printf("		Test case: \x1B[32mPASSED\033[0m\n");

	//print_printer(prn);

	return 0;
}
