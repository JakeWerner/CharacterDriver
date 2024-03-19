#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
//#include <sys/types.h>
#include <unistd.h>


#define maxBufferSize 1024


int main(){
	char *device_buffer; //Grab the device buffer
	int testFile = open("/dev/simple_character_device", O_RDWR); //Open the driver file. O_RDWR specifies read/write
	printf("Driver File Opened. Opening Test Program \n");
	char choice;
	int status = 1;
	while(status == 1){
		printf("MAIN MENU:\n");
		printf("Options: (r to read from device) (w to write to device) (s to seek device) (e to quit)\n");
		printf("Enter character: ");
		choice = getchar();
		switch (choice) {
			case 'r':
				printf("Enter the number of bytes you want to read: ");
				int bytesIn;
				scanf("%d", &bytesIn);
				device_buffer = malloc(bytesIn);
				read(testFile, device_buffer, bytesIn);
				printf("Read %d bytes from %s \n", bytesIn, device_buffer);
			break;

			case 'w':
				printf("Enter data you want to write: ");
				char writeMessage[maxBufferSize];
				scanf("%s", &writeMessage[0]);
				int length = strlen(writeMessage); 
				write(testFile, writeMessage, length);
				printf("Message written to device\n");
			break;

			case 's':
				printf("Enter an offset value: ");
				int offset;
				scanf("%d", &offset);
				printf("Enter a value for whence (0, 1, 2): ");
				int whence;
				scanf("%d", &whence);
				llseek(testFile, offset, whence);
			break;

			case 'e':
				printf("Quitting test\n");
				return 0;
				//release(testFile);
			break;
		}
	}
	return 0;
}
