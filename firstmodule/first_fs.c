#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//#define DEVICE "/sys/class/firstClass/mynull/dev"

#define DEVICE "/dev/niteechar"

int main() {

	int i,fd;
	char ch, write_buf[100], read_buf[100];
	fd = open(DEVICE, O_RDWR); // open for reading and writing 
	if (fd== -1){
		printf("file does not exist");
		exit(-1);
	}
	printf("r = read \nw = write \nenter command: ");
	scanf("%c", &ch);
	
	switch(ch) {
	
		case 'w':
			printf("enter data: ");
			scanf(" %[^\n]", write_buf);
			write(fd,write_buf, sizeof(write_buf));
			break;
		
		case 'r':
			read(fd, read_buf, sizeof(read_buf));
			printf("device: %s\n", read_buf);
			break;

		default:
			printf("please enter correct value");
			break;
	}
	close(fd);

	return 0;
}
