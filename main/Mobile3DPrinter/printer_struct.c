
#include "printer_struct.h"

void printer_get_tip(struct Printer* prn, vec3 out) {

	vec3 new = GLM_VEC3_ZERO_INIT;
	glm_mat4_mulv3(prn->links[4].absolute_mat,new,1.f,out);
}