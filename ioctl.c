#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
 
#define WR_VALUE _IOW('a','a',int32_t*) //IOCTL code for input/output/write (magic number,command number, datatype)
#define RD_VALUE _IOR('a','b',int32_t*) //IOCTL code for input/output/read
 
int main()
{
        int fd;
        int32_t value, number;
 
        printf("\nIOCTL based charector driver\n");
        fd = open("/dev/exam_device", O_RDWR);       //opening device driver file
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter the Value to send\n");
        scanf("%d",&number);
        printf("Writing Value to Driver\n");
        ioctl(fd, WR_VALUE, (int32_t*) &number); //IOCTL write call
 
        printf("Reading Value from Driver\n");
        ioctl(fd, RD_VALUE, (int32_t*) &value);  //IOCTL read call
        printf("Value is %d\n", value);
 
        printf("Closing Driver\n");
        close(fd);   //closing file
}
