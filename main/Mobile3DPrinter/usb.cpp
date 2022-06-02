#include "printer_usb.h"
#include <iostream>

InputState usb_init_state() {
	InputState new_state;

	memcpy(new_state.input_buffer,"",sizeof(new_state.input_buffer));
	new_state.input_idx = 0;

	memcpy(new_state.parser_buffer, "", sizeof(new_state.parser_buffer));
	new_state.parser_idx = 0;
	
	return new_state;
}

//Start up a serial connection
PORT usb_init() {

	PORT port = OpenPort(PORTNO);

	//Set all of the properties of the serial connection 
	//These are the arduino defaults
	if (!(SetPortBoudRate(port, 500000))) {
		printf("Setting Boud rate failed!\n");
		return NULL;
	};

	if (!(SetPortStopBits(port, CP_STOP_BITS_ONE))) {
		printf("Setting stop bits failed!\n");
		return NULL;
	};

	if (!(SetPortParity(port, CP_PARITY_NOPARITY))) {
		printf("Setting parity bits failed!\n");
		return NULL;
	}

	if (!(SetPortDataBits(port, CP_DATA_BITS_8))) {
		printf("Setting data bits failed!\n");
		return NULL;
	}

	return port;
}

//Just a wrapper for the ClosePort function
//Any other shutdown functionality can be added here
void usb_close(PORT port) {

	ClosePort(port);

	return;

}

void usb_get_input(PORT port, struct InputState* state) {
	
	//The amount of characters last read is stored in order to
	//Allow for non-full buffers
	int got_data = ReciveData(port, state->input_buffer, BUFFSIZE);
	if (!got_data) {
		printf("\x1B[31mGot no data!\033[0m\n");
		//If we don't get input, clear the buffer
		strcpy_s(state->input_buffer,"");
	}
	else {
		//For debug purposes:
		printf("Bytes read: %s", state->input_buffer);
	}
	state->input_idx = 0;
}

void usb_new_command(struct InputState* state) {
	state->parser_idx = 0;
	strcpy_s(state->parser_buffer, "");
}

void usb_add_char(struct InputState* state, char c) {
	state->parser_buffer[state->parser_idx] = c;
	state->parser_buffer[state->parser_idx + 1] = '\0';
	state->parser_idx++;

	//There are no commands which should reasonably be larger than the default BUFFSIZE of 64 chars.
	//Therefore the program will just exit gracefully
	if (state->parser_idx+1 == BUFFSIZE-1) {
		printf("Parser buffer has been overloaded. This should not happen during normal operation\n");
		exit(1);
	}
}

void usb_get_command(PORT port, struct InputState* state) {

	//How many chars of the command initliazer '<!' we have seen
	bool accepting = FALSE;
	bool saw_lt = FALSE;
	bool saw_gt = FALSE;

	//Continously read data
	while (state->input_idx < BUFFSIZE){

		//If we recieve "<!" begin accepting input
		char c = state->input_buffer[state->input_idx];
		switch (c) {
			case '<':
				//If we are not yet accepting a command, mark it
				if (!accepting) {
					saw_lt = TRUE;
				}else {
					usb_add_char(state,c);
				}
				break;
			case '!':
				//If we aren't accepting a command and have seen lt, mark it
				if (!accepting && saw_lt) {
					usb_new_command(state);
					accepting = TRUE;
					saw_lt = FALSE;
				}
				else if(accepting){
					usb_add_char(state, c);
				}
				break;
			case '>':
				//If we were accepting, end the function to allow parsing to take place
				if(accepting) saw_gt = TRUE;
				break;
			default:
				if (accepting) usb_add_char(state,c);
		}

		//Pull more data if we are out of data
		state->input_idx++;
		if (state->input_idx == BUFFSIZE){
			usb_get_input(port, state);
		}

		//Shut the function down if saw the end of a command
		//Needs to be done after pulling data to set up for next call
		if (saw_gt) {
			return;
		}

		//printf("Getting char %c, index %i\n",c,state->input_idx);
	}

	//By the end of the function we ought to have
	//A string in the parser buffer which has been stripped of the angle brackets
}

void usb_test() {

	PORT port = usb_init();
	//Check to ensure port is not NULL
	if (!port) return;

	char sendstr[BUFFSIZE] = "This text is coming from the computer";
	int senderr = SendData(port, sendstr);
	printf("senderr: %i\n", senderr);

	char teststr[BUFFSIZE] = "Test message";

	char recivestr[BUFFSIZE] = "";

	for (int i = 0; i < 20; i++) {
		int recverr = ReciveData(port, recivestr, BUFFSIZE - 1);
		printf("recv: %i\n", recverr);

		printf("Recieved: %s\n", recivestr);
	}

	usb_close(port);
}