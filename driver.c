#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

#define mem_size 1024
 
 //Define the ioctl code
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
 
int32_t value = 0;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev exam_cdev;
uint8_t *kernel_buffer;

//Function Prototypes

static int      __init ioctl_driver_init(void);
static void     __exit ioctl_driver_exit(void);
static int      exam_open(struct inode *inode, struct file *file);
static int      exam_release(struct inode *inode, struct file *file);
static ssize_t  exam_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  exam_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     exam_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = exam_read,
        .write          = exam_write,
        .open           = exam_open,
        .unlocked_ioctl = exam_ioctl,
        .release        = exam_release,
};


//This function will be called when we open the Device file

static int exam_open(struct inode *inode, struct file *file)
{
        if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0){
        printk("Cannot allocate memory to kernel\n");
        return -1;
        }
        printk("Device File Opened...!!!\n");
        return 0;
}


//This function will be called when we close the Device file

static int exam_release(struct inode *inode, struct file *file)
{
        printk("Device File Closed...!!!\n");
        return 0;
}


//This function will be called when we read the Device file

static ssize_t exam_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	copy_to_usr(buf, kernel_buffer, mem_size);        
        printk("Read Data done...\n");
        return mem_size;
}


//This function will be called when we write the Device file

static ssize_t exam_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	copy_from_usr(kernel_buffer, buf, len);
        printk("Write Data done...\n");
        return len;
}


//This function will be called when we write IOCTL on the Device file

static long exam_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        pr_info("Value = %d\n", value);
                        break;
                case RD_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
                        break;
                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
        

//Module Init function

static int __init exam_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "exam_Dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&exam_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&exam_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"exam_class")) == NULL){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
        
/*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"exam_device")) == NULL){
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

/*
** Module exit function
*/
static void __exit exam_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&exam_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(exam_driver_init);
module_exit(exam_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saquib");
MODULE_DESCRIPTION("IOCTL");
