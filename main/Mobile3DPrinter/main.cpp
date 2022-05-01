#include <stdio.h>
#include <string.h>

//I feel like I have to defend this
//The windows API and C apparently do not get along to the tune of 3000 compiler errors
//So anything which the windows API touches is C++ despite everything else being in C.
// Have fun.
extern "C" {
#include "printer_struct.h"
#include "kinematics.h"
#include "csv_parser.h"
}

#include <assert.h>
#include "printer_usb.h"


//Initial buffer size for printing path (number of points)
#define PATH_SIZE 32

//Convert sphere coordinates into a normal vector to feed into the IK algorithms
//Probably should have had that take in sphere coords to start with...
void sphere_to_normal(vec3 norm, float theta, float phi) {

	norm[0] = (float)(cos(theta) * sin(phi));
	norm[1] = (float)(sin(theta) * sin(phi));
	norm[2] = (float)(cos(theta));

	glm_vec3_normalize(norm);
}

//Output a point to the controller in the required format
void output_point(struct Printer* prn, bool extrude) {

	//Precise size of the outgoing string
	char outgoing[62];

	//Extrusion var to send
	char ex;
	if (extrude) {
		ex = 't';
	}
	else {
		ex = 'f';
	}

	snprintf(outgoing,62,"<!D(%+010.4f,%+010.4f,%+010.4f,%+010.4f,%+010.4f,%c)>",prn->motors[0].angle,prn->motors[1].angle,prn->motors[2].angle,prn->motors[3].angle,prn->motors[4].angle,ex);

	printf("Outgoing data: %s\n",outgoing);
}

int main(int argc,char* argv[]) {

	//Get the CSV file path from command line arguments

	/*
	//Check to ensure there is a filepath
	if (argc < 2) {
		printf("ERROR: No filepath provided\n");
		return 0
	}

	char* filepath = argv[1];
	printf("Printing %s",filepath);
	*/

	char filepath[64] = "..\\ToolPath.csv";

	//Parse the CSV file
	struct Path print_path;
	print_path.points = (struct Point*) malloc(sizeof(struct Point)*PATH_SIZE); //Say your prayers boys we doing dynamic memory
	print_path.size = 0;
	print_path.cap = PATH_SIZE;

	int parse_fail = csv_parse(&print_path,filepath);
	if (parse_fail) {
		free(print_path.points);
		printf("Unexpected error: File parsing failed\n");
		return 0;
	}

	printf("Loaded file: %s\n",filepath);

	int print_path_idx = 0;

	//***************
	//Printer Setup

	//Create a new printer struct and hard set the angles to zero
	struct Printer printer;
	
	//Uses defines for printer structure. See printer_defines.h
	//Has all motor angles set to zero by default
	printer = generate_printer();
	forward_kinematics(&printer);

	vec3 tip;
	printer_get_tip(&printer, tip);
	print_vec3(tip);

	int extruding = 0;
	
	//***************
	//Program state

	int error = 0;
	bool running = TRUE;
	bool eof_called = FALSE;

	//***************
	//Serial input

	//Get serial input
	PORT port = usb_init();
	if (!port) {
		printf("Failed to open the COM%i port. Please make sure the printer is connected and on the correct port\n",PORTNO);
		exit(1);
	}

	//Send the homing command to the printer, also syncs it up
	SendData(port, "<!h>");

	//Initializes the input
	struct InputState i_state = usb_init_state();

	//Primes the input state buffers and gets the first command
	usb_get_input(port, &i_state);
	usb_get_command(port, &i_state);

	//Wait until we get homing confirmation
	//Literally can't do anything until we get it
	while (TRUE) {

		printf("Waiting for homing confirmation!\n");

		if (i_state.parser_buffer[0] == 'H') {
			printf("Homing completed\n");
			break;
		}

		usb_get_command(port, &i_state);
	}
	
	//Begin main processing loop

	while (running && !error) {

		char c = i_state.parser_buffer[0];

		//Extract the message payload
		char payload[BUFFSIZE];

		//Message format for all of these is a single char before a parenthesis
		// e.g. 'm(message)'
		//We know how much to grab due to the parser_idx being the index of the end
		//We know we can skip the first two chars
		//Strcopy did not work as I expected so we'll just uh do this instead ;)

		//printf("%s\n", i_state.parser_buffer);

		for (int i = 2; i < (i_state.parser_idx); i++) {
			char chr = i_state.parser_buffer[i];

			if (chr == ')') {
				payload[i-2] = '\0';
			}
			else {
				payload[i-2] = chr;
			}
		}

		switch (c) {
			case 'd': {
				//Handle sending an amount of data
				//Aka the slightly more difficult part
				int num_datas = atoi(payload);
				printf("I am supposed to print %i datas\n", num_datas);
				
				for (int i = 0; i < num_datas; i++) {
					
					struct Point point = print_path.points[print_path_idx];

					//Put the print path data into vectors
					vec3 position = { point.x,point.y,point.z };
					vec3 normal;

					printf("Attenpting to print: %f %f %f\n",point.x,point.y,point.z);

					sphere_to_normal(normal, point.theta, point.phi);

					//Calculate and send the required data the required number of times
					int ik_err = inverse_kinematics(&printer,position,normal);
					
					//Handle any issues with the IK
					if (ik_err) {
						
						for (int i = 0; i < NUM_MOTORS; i++) {
							print_motor(&printer.motors[i]);
						}

						printf("Error: IK out of bounds\n");
						//SendData(port,"<!s>");
						error = TRUE;
						break;
					}

					output_point(&printer,point.extrusion);

					//If we need to do a check for data recieved response, it goes here

					print_path_idx++;
					//If we've hit the end of the file, we stop running and tell the controller we're at EOF
					if (print_path_idx == print_path.size) {

						SendData(port,"<!e>");
						printf("End of file reached!\n");

						eof_called = TRUE;
						running = FALSE;
						break;
					}
				}

				break;
			}
			case 's':
				printf("Error: %s\n", payload);
				error = TRUE;
				break;
			case 'm':
				printf("%s\n",payload);
				break;
		}

		if (!error) {
			usb_get_command(port, &i_state);
		}
	}

	//Handle waiting for EoF confirmation
	//Even though it probably doesn't matter if we get it
	if (eof_called) {

		while (TRUE) {

			if (i_state.parser_buffer[0] == 'E') {
				printf("End of file reached. Thank you for choosing The Hulk for all your Mobile 3D printing needs <(-_-<)\n");
				break;
			}

			usb_get_command(port, &i_state);
		}
	}


	//******
	//Shutdown

	//Free the memory we were using to dynamically store the printing path data
	free(print_path.points);

	//Exit the USB functionality
	usb_close(port);

	return 0;
}