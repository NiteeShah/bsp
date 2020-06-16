/*
 * linux/drivers/char/myfirst.c
 * 
 * implement char driver
 * Register, Deregister a device 
 * Device File Operation for - open, read,write, close operations 
 * Module init and exit 
 */

#include <linux/init.h>
#include <linux/fs.h> /* for character driver support*/
#include <linux/module.h>
#include <linux/cdev.h>

struct class *myfirst_class;
/*File Operations methods - open, read, write and relase */

ssize_t myfirst_read(struct file *myfirst_fs, char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_ERR "Nitee: Read from File %s\n",__FUNCTION__);
    return 0;
}

ssize_t myfirst_write(struct file *myfirst_fs, const char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_ERR "Nitee: Write to File %s\n",__FUNCTION__);
    return count;
}

int myfirst_open(struct inode *minode, struct file *myfirst_fs)
{
	printk(KERN_ERR "Nitee: Open File %s\n",__FUNCTION__);
	return 0;
}

int myfirst_release(struct inode *minode, struct file *myfirst_fs)
{
    printk(KERN_ERR "Nitee: Release File %s\n",__FUNCTION__);
    return 0;
}


/* File Operations found in lib/modules/build/linus/fs.h */
static const struct file_operations myfirst_fs = {
	.owner 		= THIS_MODULE,	
    .open       = myfirst_open,
	.read		= myfirst_read,
	.write		= myfirst_write,
    .release    = myfirst_release,
};


/* init opertaion */
static int __init myfirst_init(void)
{
	/*
	 * Register Device, Create Class and Create device
	 */
	printk(KERN_ERR "Nitee: Module init %s\n",__FUNCTION__);
//	register_chrdev_region(dev, max_raw_minors, "raw"); Minor Number must be between 1 and 65536
	register_chrdev_region(220, 2, "myfirst");
	myfirst_class = class_create(THIS_MODULE, "myfirst");
        device_create(myfirst_class, NULL, 220, NULL, "myfirst");

	return 0;
}


static void __exit myfirst_exit(void)
{
	/*
	 * Destroy Device, Destroy Class and Unregister Device. 
	 * */
	printk(KERN_ERR "Nitee: Module exit %s\n",__FUNCTION__);
	device_destroy(myfirst_class, 220);
        class_destroy(myfirst_class);
	unregister_chrdev_region(220, 2);
}

module_init(myfirst_init);
module_exit(myfirst_exit);
MODULE_LICENSE("GPL");
