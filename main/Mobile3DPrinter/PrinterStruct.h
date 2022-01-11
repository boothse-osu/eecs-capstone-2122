#ifndef PRINTER_STRUCT
#define PRINTER_STRUCT

struct Motor {
	double angle;
	double max_angle;
	double min_angle;
};

struct Link{
	double length;
	struct Motor;
	//Vector3 storing the coordinates of the end of the link
	//Vector3 storing the coordinates of the start of the link
};

struct Printer{
	struct Link* links;
};

#endif