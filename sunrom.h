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
 * sunrom.h
 *
 *  Created on: 11 May 2017
 *      Author: Ryan
 */
#ifndef SUNROM_H
#define SUNROM_H

class SUNROM
{
public:
	// Constructor
	SUNROM(char* serialPort, float scaling); 
	
	// Destructor
	~SUNROM();
	
	// Public functions
	void init_serial(char* modemDevice); // Initialize serial interface.
	void close_serial(void); // Close fd
	void send_serial(char *data, unsigned int len); // Send data buffer over serial
	void update_load(void); // Read and store load cell value in load private data
	float get_load(void); // Return load private data

private:
	// Private functions
	bool receive_serial(void);
	
	// Private data
	int fd; // serial file descriptor
	float load; // current load value
	float scalingFactor; // scaling factor for raw adc to kg
};
#endif