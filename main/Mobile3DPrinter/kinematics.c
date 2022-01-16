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
	for (int i = 0; i < 5; i++) {

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
	for (int i = 0; i < 5; i++) {

		mat4 prev_matrix;

		//Special case for the first matrix; use the identity matrix as previous matrix
		//Previous matrix is the previous absolute matrix
		if (i == 0) {
			glm_mat4_identity(prev_matrix);
		}
		else {
			glm_mat4_copy(prn->links[i - 1].absolute_mat, prev_matrix);
		}

		glm_mat4_mul(prn->links[i].current_mat,prev_matrix,prn->links[i].absolute_mat);

	}
}
