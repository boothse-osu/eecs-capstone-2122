#include "printer_usb.h"
#include <iostream>

#define BUFFSIZE 64

void usb_test() {

	PORT port = OpenPort(4);

	int flusherr;
	flusherr = FlushFileBuffers(port);
	printf("flusherr: %i\n", flusherr);

	if (!(SetPortBoudRate(port, 9600))) {
		printf("Setting Boud rate failed!\n");
		return;
	};

	if (!(SetPortStopBits(port, CP_STOP_BITS_ONE))) {
		printf("Setting Stop bits failed!\n");
		return;
	};

	if (!(SetPortParity(port, CP_PARITY_NOPARITY))) {
		printf("Setting parity bits failed!\n");
		return;
	}

	if (!(SetPortDataBits(port, CP_DATA_BITS_8))) {
		printf("Setting parity bits failed!\n");
		return;
	}

	char sendstr[BUFFSIZE] = "This text is coming from the computer";
	int senderr = SendData(port, sendstr);
	printf("senderr: %i\n", senderr);

	char teststr[BUFFSIZE] = "Test message";

	char recivestr[BUFFSIZE] = "";

	for(int i = 0;i < 20;i++){
		int recverr = ReciveData(port, recivestr, BUFFSIZE-1);
		printf("recv: %i\n", recverr);

		printf("Recieved: %s\n", recivestr);
	}

	flusherr = PurgeComm(port, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	printf("flusherr: %i\n", flusherr);

	ClosePort(port);

	return;
}