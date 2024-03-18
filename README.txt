Jacob Werner
CUID: 108968839
jawe2981@colorado.edu

CSCI 3753 - PA 2

Makefile
	-Located in /home/kernel/modules

SCDTest.c
	-Located in /home/kernel/modules
	-Test program that tests the implementation of the driver. Can be compiled using gcc. Make sure that the module is installed before compiling and running the test program. Run the test file using ./(name given to executable) and then the program will interact through the terminal. A warning may pop up on compiliation but the program and the driver works so there should be no problem.

simple_character_device
	-Driver file located in /dev
	-Do not need to place this but to install, go to the dev directory and use sudo mknod -m 777 /dev/simple_character_device c 240 0. This must occur before install of the driver. 

simple_character_driver.c
	-Located in /home/kernel/modules
	-To install, first place into the correct directory (and make sure Makefile is in the same directory). Then use the command: make -C /lib/modules/$(uname -r)/build M=$PWD. Upon successful compilation, use the command: sudo insmod simple_character_driver.ko to finally install the LKM. To check for a successful install, type dmesg into the terminal and a message should pop up saying the device was correctly registered. 


