#include "printer_usb.h"
#include <stdio.h>

int main() {

	PORT port = usb_init();

	//Send data to start the stream of data
	SendData(port,"*");

	struct InputState state = usb_init_state();

	usb_get_input(port,&state);

	usb_get_command(port, &state);

	printf("Parser buffer: %s\n",state.parser_buffer);

	usb_new_command(&state);

	usb_get_command(port, &state);

	printf("Parser buffer: %s\n", state.parser_buffer);

	usb_close(port);

	return 0;
}

