#include "printer_usb.h"
#include <iostream>

#define BUFFSIZE 256

void usb_test() {

	PORT port = OpenPort(4);

	int flusherr = PurgeComm(port, PURGE_RXCLEAR | PURGE_TXCLEAR);
	printf("flusherr: %i\n",flusherr);

	if (!(SetPortBoudRate(port, 9600))) {
		printf("Setting Boud rate failed!\n");
		return;
	};

	char sendstr[BUFFSIZE] = "This text is coming from the computer";
	char recivestr[BUFFSIZE]= "0";
	int senderr = SendData(port, sendstr);
	printf("senderr: %i\n", senderr);
	int recverr = ReciveData(port, recivestr, BUFFSIZE);
	printf("recv: %i\n", recverr);

	printf("Recieved: %s", recivestr);
	//strcpy_s(recivestr,"test");

	ClosePort(port);

	return;
}