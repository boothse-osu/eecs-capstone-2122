#ifndef PRINTER_DEFINES
#define PRINTER_DEFINES

#include <cglm.h>

//Scaling factor for floating points
#define FLOAT_SCALE 1000.f

//Defines for the inverse kinematics
//All of this is used by the generate_printer function in printer_struct.c

//Number of links
//Should be 5, but might be higher if there are links without motors which are important
#define NUM_LINKS 6

//Link home position vectors
#define LINK0_HOME (vec3){1.f,1.f,1.f}
#define LINK1_HOME (vec3){1.f,1.f,1.f}
#define LINK2_HOME (vec3){1.f,1.f,1.f}
#define LINK3_HOME (vec3){1.f,1.f,1.f}
#define LINK4_HOME (vec3){1.f,1.f,1.f}

#define LINK_HOME_LIST {LINK0_HOME,LINK1_HOME,LINK2_HOME,LINK3_HOME,LINK4_HOME}

//Whether or not the link has a prismatic joint at its base 1 for yes, 0 for no
#define LINK0_PRISMATIC 1
#define LINK1_PRISMATIC 1
#define LINK2_PRISMATIC 1
#define LINK3_PRISMATIC 0
#define LINK4_PRISMATIC 0

#define LINK_PRISMATIC_LIST {LINK0_PRISMATIC,LINK1_PRISMATIC,LINK2_PRISMATIC,LINK3_PRISMATIC,LINK4_PRISMATIC}

//If prismatic, the axis the link moves in
//If rotational, the axis the link rotates around
#define LINK0_AXIS (vec3){1.f,0.f,0.f} //X
#define LINK1_AXIS (vec3){0.f,0.f,1.f} //Z
#define LINK2_AXIS (vec3){0.f,1.f,0.f} //Y
#define LINK3_AXIS (vec3){0.f,0.f,1.f} //Z rot
#define LINK4_AXIS (vec3){1.f,0.f,0.f} //X rot

#define LINK_AXIS_LIST {LINK0_AXIS,LINK1_AXIS,LINK2_AXIS,LINK3_AXIS,LINK4_AXIS}

//Link movement ratios
//If rotational, the ratio of motor movement in radians to link rotation in radians
//If prismatic, the ratio of motor movement in radians to linear distance traveled
#define LINK0_RATIO 1.f
#define LINK1_RATIO 1.f
#define LINK2_RATIO 1.f
#define LINK3_RATIO 1.f
#define LINK4_RATIO 1.f

#define LINK_RATIO_LIST {LINK0_RATIO,LINK1_RATIO,LINK2_RATIO,LINK3_RATIO,LINK4_RATIO}

//Number of motors
//Should be same as number of links, but plausable that it might not be the case
#define NUM_MOTORS 5
//Will need to change the motor lists as well if changing

//Motor bounds (radians)
//Max
#define MOTOR0_MAX 1.f
#define MOTOR1_MAX 1.f
#define MOTOR2_MAX 1.f
#define MOTOR3_MAX 1.f
#define MOTOR4_MAX 1.f

#define MOTOR_MAX_LIST {MOTOR0_MAX,MOTOR1_MAX,MOTOR2_MAX,MOTOR3_MAX,MOTOR4_MAX}
//Min
#define MOTOR0_MIN 1.f
#define MOTOR1_MIN 1.f
#define MOTOR2_MIN 1.f
#define MOTOR3_MIN 1.f
#define MOTOR4_MIN 1.f

#define MOTOR_MIN_LIST {MOTOR0_MIN,MOTOR1_MIN,MOTOR2_MIN,MOTOR3_MIN,MOTOR4_MIN}

#endif
