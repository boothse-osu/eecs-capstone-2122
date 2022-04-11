#ifndef USB
#define USB

#include "CSerialPort.h"

//Size of the input character buffer.
#define BUFFSIZE 64
//Which COMM port the program tries to use. May need to be changed from machine to machine.
#define PORTNO 4

struct InputState {
	//Buffer for uncleaned input
	char input_buffer[BUFFSIZE];
	//Index of where we last were in the buffer
	int input_idx;

	//Buffer for cleaned input to parse
	char parser_buffer[BUFFSIZE];
	//Index of where the next character will go in the parser buffer
	int parser_idx;

	//Whether or not we are still putting things into the
	//parsing buffer
	bool accepting;

	//The number of chars read in the last read
	int last_read;
};

//Begins a serial connection
//Returns NULL if it cannot
PORT usb_init();

//Ends a serial connection
void usb_close(PORT);

//Reads input utill it is able to put a full command in the parser buffer
void usb_get_command(PORT,struct InputState*);

//Test function for the USB code
void usb_test();

#endif
