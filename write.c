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
        
        printf("Enter the Value to send\n");
        scanf("%d",&number);
        printf("Writing Value to Driver\n");
        ioctl(fd, WR_VALUE, (int32_t*) &number); //IOCTL write call
 
        printf("Closing Driver\n");
        close(fd);   //closing file
}
