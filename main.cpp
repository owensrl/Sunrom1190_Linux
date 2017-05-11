// Main program to test serial communication between Beaglebone Black and Sunrom1190
// Last modified on 5/11/2017 by: Ryan Owens
#include <stdio.h>
#include "sunrom.h"
#include <unistd.h> // for usleep

int main(void) {

	// variables
	int loop = 1;
	float load = 0;
	
	SUNROM loadCell((char*)"/dev/ttyO1", 0.38); // 0.38 is default 40kg load cell
	                                        // scaling factor. Adjust for different
	                                        // load cells.
	// Main loop 
	while(loop) {
		loadCell.update_load();
		load = loadCell.get_load();
		printf("%.3f Kg\n\r", load);
		usleep(100000);
	}
	loadCell.close_serial();
}
