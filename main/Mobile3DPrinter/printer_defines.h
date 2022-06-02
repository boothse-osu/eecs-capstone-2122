#ifndef PRINTER_DEFINES
#define PRINTER_DEFINES

#include <cglm.h>

//Scaling factor for floating points
#define FLOAT_SCALE 1000.f

//Scaling factor for rounding. 10 would round off to the nearest 10th.
#define ROUND_PRECISION 10000

//Determines whether or not the angle bounds checker is on
//For debug purposes
#define ANGLE_BOUNDS 0

//Determines whether the IK will consider the normal
//For debug purposes

#define IK_NORMALS 1

//Defines for the inverse kinematics
//All of this is used by the generate_printer function in printer_struct.c

//Number of links
//Should be 5, but might be higher if there are links without motors which are important
#define NUM_LINKS 6

//Positions of the end of each link
//0,0,0 is the end of the X motor
#define LINK0_END (vec3){6.183f,178.998f,50.5f} //X
#define LINK1_END (vec3){-77.496f,48.498f,148.25f} //Z
#define LINK2_END (vec3){-104.848f,278.748f,106.242f} //Y
#define LINK3_END (vec3){-104.903f,286.24f,113.75f} //YX
#define LINK4_END (vec3){-64.458f,279.043f,113.75f} //YZ
#define LINK5_END (vec3){-64.458f,279.043f,45.95f} //Extra bit at the end of the printer

#define LINK_END_LIST {LINK0_END,LINK1_END,LINK2_END,LINK3_END,LINK4_END,LINK5_END}

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
//If prismatic, the ratio of motor movement in radians to linear distance traveled in mm
#define LINK0_RATIO 1.1459f
#define LINK1_RATIO 1.1459f
#define LINK2_RATIO 6.11f //Sus
//Rotational joints
#define LINK3_RATIO 1.f
#define LINK4_RATIO 1.f

#define LINK_RATIO_LIST {LINK0_RATIO,LINK1_RATIO,LINK2_RATIO,LINK3_RATIO,LINK4_RATIO}

//Number of motors
//Should be same as number of links, but plausable that it might not be the case
#define NUM_MOTORS 6
//Will need to change the motor lists as well if changing

//Motor bounds (radians)
//Max
#define MOTOR0_MAX 570.f / LINK0_RATIO
#define MOTOR1_MAX 340.f / LINK1_RATIO
#define MOTOR2_MAX 560.f / LINK2_RATIO
#define MOTOR3_MAX (M_PI*2.0) * LINK3_RATIO
#define MOTOR4_MAX (M_PI*2.0) * LINK4_RATIO
//Dummy motor. Should never be moved.
#define MOTOR5_MAX 0.f

#define MOTOR_MAX_LIST {MOTOR0_MAX,MOTOR1_MAX,MOTOR2_MAX,MOTOR3_MAX,MOTOR4_MAX}
//Min
#define MOTOR0_MIN -10.f / LINK0_RATIO
#define MOTOR1_MIN -10.f / LINK1_RATIO
#define MOTOR2_MIN -560.f / LINK2_RATIO
#define MOTOR3_MIN (-M_PI*2.0) * LINK3_RATIO
#define MOTOR4_MIN (-M_PI*2.0) * LINK4_RATIO
//Dummy motor. Should never be moved.
#define MOTOR5_MIN 0.f

#define MOTOR_MIN_LIST {MOTOR0_MIN,MOTOR1_MIN,MOTOR2_MIN,MOTOR3_MIN,MOTOR4_MIN}

#endif
