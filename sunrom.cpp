/*
	Copyright 2017 Ryan Owens

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

/*
 * sunrom.cpp
 *
 *  Created on: 11 May 2017
 *      Author: Ryan
 */
#include "sunrom.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Settings
#define BAUDRATE B9600   // Change as needed, keep B

#define _POSIX_SOURCE 1 /* POSIX compliant source */

// Maximum receive buffer size
#define SERIAL_RX_BUFFER_SIZE 32

// Global buffer
static char dataBuffer[SERIAL_RX_BUFFER_SIZE];

SUNROM::SUNROM(char* serialPort, float scaling) : scalingFactor(scaling), load(0)
{
	init_serial(serialPort);
	send_serial((char*)"R", 1); // set output to RAW mode
}

SUNROM::~SUNROM()
{
}

void SUNROM::init_serial(char* modemDevice) {
	struct termios newtio;
	
	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;

    /*  Raw output  */
    newtio.c_oflag = 0;
    
    newtio.c_cc[VTIME] = 1;
    newtio.c_cc[VMIN]  = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    newtio.c_lflag = ICANON;
	
    // Load the pin configuration
	/* Open modem device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C. */
    fd = open(modemDevice, O_RDWR | O_NOCTTY| O_NONBLOCK);
    if (fd < 0) { perror(modemDevice); exit(-1); }
    
    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
}

void SUNROM::close_serial(void) {
	close(fd);
}

void SUNROM::update_load(void) {
	if (receive_serial()) {
		// scale raw adc to kg
		load = atof(dataBuffer);
		load +=  load * scalingFactor;
	}
}

float SUNROM::get_load(void) {
	return load;
}

void SUNROM::send_serial(char *data, unsigned int len) {

	// Wait for the previous transmission to finish.
	tcdrain(fd);

	// Send the data over Serial
	write(fd, data, len);
}

bool SUNROM::receive_serial(void) {
	int ret;
	static char rxBuffer[SERIAL_RX_BUFFER_SIZE];
	// Read characters from Serial and put them in a buffer
	ret = read(fd, rxBuffer, SERIAL_RX_BUFFER_SIZE);
	if (ret) {
		strncpy(dataBuffer, rxBuffer, 6);
		return true;
	}
	else
		return false;
}
