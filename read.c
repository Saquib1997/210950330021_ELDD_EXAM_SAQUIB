#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
 
 
int main()
{
        int fd;
        char readbuff[100];
 
        fd = open("/dev/exam_device", O_RDWR);       //opening device driver file
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Reading Value from Driver\n");
        read(fd, readbuff, 100);  //user read call
        printf("Value is %s\n", readbuff);
 
        printf("Closing Driver\n");
        close(fd);   //closing file
}
