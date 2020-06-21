#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define DEVICE "/dev/myfirsttimer"
#define TIMER_START _IOW('a','1',int32_t*)
#define TIMER_STOP _IOR('b','2',int32_t*)

int main() {

	int i,fd;
	int32_t val;
	char ch, write_buf[100], read_buf[100];
	fd = open(DEVICE, O_RDWR); // open for reading and writing 
	if (fd== -1) {
		printf("file does not exist");
		exit(-1);
	}
	printf("a = timer_start \nb = timer_stop \nenter command: ");
	scanf("%c", &ch);
	
	switch(ch) {	
	case 'a':
		printf("timer starts for 5 secs ");
		ioctl(fd,TIMER_START, sizeof(write_buf));
		break;
	case 'b':
		ioctl(fd, TIMER_STOP, sizeof(read_buf));
		printf("timer stopped");
		break;
	default:
		printf("please enter correct value");
		break;
	}
	close(fd);

	return 0;
}
