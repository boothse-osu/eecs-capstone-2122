#include <iostream>
#include <string>

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
void output_point(std::string* out, struct Point point) {

	//Extrusion var to send
	char ex;
	if (point.extrusion) {
		ex = 't';
	}
	else {
		ex = 'f';
	}

	char outgoing[63];
	snprintf(outgoing,63,"<!D(%+010.4f,%+010.4f,%+010.4f,%+010.4f,%+010.4f,%c)>",point.x,point.y,point.z,point.theta,point.phi,ex);

	std::string line(outgoing);

	*out += line;
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

	char filepath[64] = "..\\Circle_Path (50 Points)+Distance.csv";

	//Parse the CSV file
	struct Path print_path;
	print_path.raw_points = (struct Point*) malloc(sizeof(struct Point)*PATH_SIZE); //Say your prayers boys we doing dynamic memory
	print_path.size = 0;
	print_path.cap = PATH_SIZE;

	int parse_fail = csv_parse(&print_path,filepath);
	if (parse_fail) {
		free(print_path.raw_points);
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
	printf("Initial tip position: ");
	print_vec3(tip);

	int extruding = 0;
	
	//****************
	//Pre-compute the print path

	//Initialize array of motor states

	printf("Pre-computing inverse kinematics\n");

	print_path.points = (struct Point*)malloc(sizeof(struct Point) * print_path.size);

	for (int i = 0; i < print_path.size; i++) {

		struct Point point = print_path.raw_points[i];

		vec3 position = { point.x,point.y,point.z };
		vec2 normal = {point.theta,point.phi};

		vec3 position_delta;
		vec2 norm_delta;

		int ik_err = inverse_kinematics(&printer,position,normal,position_delta,norm_delta);

		//Handle any issues with the IK
		if (ik_err) {

			for (int i = 0; i < NUM_MOTORS; i++) {
				print_motor(&printer.motors[i]);
			}

			printf("Error: IK out of bounds\n");
			exit(1);
		}

		//Position values
		print_path.points[i].x = position_delta[0];
		print_path.points[i].y = position_delta[1];
		print_path.points[i].z = position_delta[2];

		//Normal values
		print_path.points[i].theta = norm_delta[0];
		print_path.points[i].phi = norm_delta[1];

		//Extrusion
		print_path.points[i].extrusion = print_path.raw_points[i].extrusion;

		//Could probably refactor the IK code to take in a Point struct at this point
	}

	free(print_path.raw_points);

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
	printf("Sending homing request!\n");

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

		printf("%i: %s\n", i_state.parser_idx,  i_state.parser_buffer);

		for (int i = 2; i < (i_state.parser_idx); i++) {
			char chr = i_state.parser_buffer[i];

			if (chr == ')') {
				payload[i-2] = '\0';
			}
			else {
				payload[i-2] = chr;
			}
		}

		usb_new_command(&i_state);

		switch (c) {
			case 'd': {
				//Handle sending an amount of data
				//Aka the slightly more difficult part
				int num_datas = atoi(payload);
				printf("I am supposed to print %i points\n", num_datas);

				std::string out_data;
				
				for (int i = 0; i < num_datas; i++) {

					struct Point point = print_path.points[print_path_idx];

					printf("Attempting to print: %f %f %f\n",point.x,point.y,point.z);
					
					output_point(&out_data,point);

					//If we need to do a check for data recieved response, it goes here
					
					print_path_idx++;
					
					//If we've hit the end of the file, we stop running and tell the controller we're at EOF
					if (print_path_idx == print_path.size) {
						eof_called = TRUE;
						running = FALSE;
						break;
					}
				}

				//Send the points all at once
				char* out = new char[out_data.length() + 1];
				std::strcpy(out, out_data.c_str());
				printf("Outgoing data: %s", out);
				SendData(port, out);

				free(out);

				if (eof_called) {
					SendData(port, "<!e>");
					printf("End of file reached!\n");
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
			default:
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