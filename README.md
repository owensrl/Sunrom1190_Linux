# Sunrom1190_Linux

This a library for serial communication between a Sunrom1190 load cell amplifier
and linux computers. The provided example in main.cpp uses a beaglebone black.
Change the serial port in init function for use with other linux computers.
Raw mode is enabled by default. Set the scaling factor by passing it as 
and argument to the init function. Other modes can be enabled by sending
the appropriate serial commands. Full documentation of the Sunrom1190
serial interface can be found at :
http://www.sunrom.com/p/load-cell-amplifier-for-weighing-scale-serial-output
