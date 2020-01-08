/**
 * @file   mydevice.c
 * @author may.sincerity
 * @date   6 March 2019
 * @version 0.1
 * @brief   A simple character device module
 */

#include <linux/init.h>           //__init __exit
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>          //copy to user
#include <linux/mutex.h>
#define  DEVICE_NAME "MyCharDev"
#define  CLASS_NAME "Char"

//version control  ...it seems that it is not necessary
#if CONFIG_MODVERSIONS == 1
#define MODVERSIONS
#include <linux/modversions.h>
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("may sincerity");
MODULE_DESCRIPTION("A simple linux char device");
MODULE_VERSION("0.1");

static int    majorNumber;
static char   message[1024] = {0};
static short  sizeOfMessage;
static int    devOpenTimes = 0;
static DEFINE_MUTEX(mydeviceMutex);//declare a mutex for mydevice

static struct class*  mydeviceClass  = NULL;
static struct device* mydeviceDevice = NULL;

static int my_open(struct inode *inodep, struct file *filep);
static int my_release (struct inode *inodep, struct file *filep);
static ssize_t my_read (struct file *filep, char *buffer , size_t length, loff_t *offset);
static ssize_t my_write (struct file *filep,const char *buffer , size_t length, loff_t *offset);

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release
};

/** 
 *  defined in linux/fs.h
 */
static int my_open(struct inode *inodep, struct file *filep) {
    if(!mutex_trylock(&mydeviceMutex)){
    	printk("mydevice is being used by another process.\n");
    	return -1;
    }
    devOpenTimes++;
    printk("mydevice has been opened %d time(s)\n", devOpenTimes);
    return 0;
}

static int my_release (struct inode *inodep, struct file *filep) {
    mutex_unlock(&mydeviceMutex);
    printk("mydevice closed successfully.\n");
    return 0;
}

/** 
 *  @param buffer write data
 *  copy_to_user ( * to, *from, size)
 */
static ssize_t my_read (struct file *filep, char *buffer , size_t length, loff_t *offset) {
    size_t bToCp = length > sizeOfMessage ? sizeOfMessage : length;
    size_t bNotCp = 0;
    if(!bToCp) return 0;
    bNotCp = copy_to_user(buffer, message, bToCp);

    if(bToCp - bNotCp)
    {
        printk("Sent %zu characters to the user\n",bToCp - bNotCp);
    }

    if(bNotCp){
        printk("Failed to send %zu characters to the user\n", bNotCp);
        return -1;
    }
    sizeOfMessage = 0;
    return bToCp;
}


static ssize_t my_write (struct file *filep,const char *buffer , size_t length, loff_t *offset) {
    const size_t maxLen = 1024 - 1;//NULL char
    size_t bToCp = length > maxLen ? maxLen: length;
    size_t bNotCp = 0;
    bNotCp = copy_from_user(message, buffer, bToCp);
    sizeOfMessage = strlen(message);
    printk("Received %d characters from the user\n", sizeOfMessage);
    if(bNotCp){
        printk("Failed to receive %zu characters!\n", bNotCp);
        return -1;
    }
    return bToCp;
}



static int __init MyCharDev_init(void){
   printk("Initializing the MyCharDev LKM\n");

   //get a majorNumber automatically
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk("Failed to register a major number\n");
      return majorNumber;
   }
   printk("Registered successfully with major number %d\n", majorNumber);

   //device class
   mydeviceClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(mydeviceClass)){
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk("Failed to register device class\n");
      return PTR_ERR(mydeviceClass);
   }
   printk("Device class registered successfully\n");

   //device driver
   mydeviceDevice = device_create(mydeviceClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(mydeviceDevice)){
      class_destroy(mydeviceClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk("Failed to create the device\n");
      return PTR_ERR(mydeviceDevice);
   }
   printk("Device class created successfully\n");
   
   mutex_init(&mydeviceMutex);

   return 0;
}

/** __exit 
 *  only used in LKM
 */
static void __exit MyCharDev_exit(void){
   device_destroy(mydeviceClass, MKDEV(majorNumber, 0));
   class_unregister(mydeviceClass);
   class_destroy(mydeviceClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk("mydevice removed successfully!\n");
}

module_init(MyCharDev_init);
module_exit(MyCharDev_exit);
