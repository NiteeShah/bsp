#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
int ret; //hold return values of the functions 

/* Create a structure for reading and writing from and to User space */

struct us_device {
	char data[100];
}virtual_device;

static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_ALERT "Nitee Driver: open()\n");
  return 0;
}
  static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_ALERT "Nitee Driver: close()\n");
  return 0;
}
  static ssize_t my_read(struct file *f, char __user *buf, size_t
  len, loff_t *off)
{
  printk(KERN_ALERT "Nitee Driver: read()\n");
  ret = copy_to_user(buf,virtual_device.data,len);
  return ret;
}
  static ssize_t my_write(struct file *f, const char __user *buf,
  size_t len, loff_t *off)
{
  printk(KERN_ALERT "Nitee Driver: write()\n");
  ret = copy_from_user(virtual_device.data,buf,len);
  return len;
}
  static struct file_operations first_fops =
{
  .owner   = THIS_MODULE,
  .open    = my_open,
  .release = my_close,
  .read    = my_read,
  .write   = my_write
};
  
static int __init first_char_driver_init(void) /* Constructor */
{
	printk(KERN_INFO "Nitee : first Char Driver registered");
	if (alloc_chrdev_region(&first, 0, 3, "firstChar") < 0)
	{
		return -1;
	}
	  if ((cl = class_create(THIS_MODULE, "firstClass")) == NULL)
	  {
	    unregister_chrdev_region(first, 1);
	    return -1;
	  }
	    if (device_create(cl, NULL, first, NULL, "mynull") == NULL)
	  {
	    class_destroy(cl);
	    unregister_chrdev_region(first, 1);
	    return -1;
	  }
	    cdev_init(&c_dev, &first_fops);
	    if (cdev_add(&c_dev, first, 1) == -1)
	  {
	    device_destroy(cl, first);
	    class_destroy(cl);
	    unregister_chrdev_region(first, 1);
	    return -1;
	  }

	return 0;
}
 
static void __exit first_char_driver_exit(void) /* Destructor */
{
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 3);
	printk(KERN_INFO "Nitee: First char dev unregistered");
}
 
module_init(first_char_driver_init);
module_exit(first_char_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nitee Shah");
MODULE_DESCRIPTION("Our First Character Driver");

