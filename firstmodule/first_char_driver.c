#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h> //copy_to/from_user()
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/workqueue.h> /* for work queue */
#include <linux/slab.h> /* for kmalloc() */

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
int ret; //hold return values of the functions 
int retval; // the the retuen value of timer functions 

/*Define IOCTL code arguments  */
#define TIMER_START _IOW('a','1',int32_t*)
#define TIMER_STOP _IOR('b','2',int32_t*)
#define SCHEDULE _IOW('c','3',int32_t*)

/* Define Timer macro */
#define TIMEOUT 5000 //miliseconds

/* Create a struct for timer_list */
static struct timer_list my_timer;

/* Timer Callback function*/

void timer_callback(struct timer_list *timer)
{

	printk("Nitee: Inside timer callback function (%ld).\n", jiffies);
}

/* Declare work and workqueue and  Define work function (the handler) */

static struct workqueue_struct *wq;
static void work_handler(struct work_struct *work);
static DECLARE_DELAYED_WORK(my_work, work_handler);

struct work_data {
	struct work_struct my_work;
	int the_data;
};

static void work_handler(struct work_struct *work)
{
	struct work_data * my_data = container_of(work,
	struct work_data, my_work);
	printk("Nitee Work queue module handler: %s, data is %d\n",__FUNCTION__, my_data->the_data);
	kfree(my_data);
}


/* Create a structure for reading and writing from and to User space */

struct us_device {
	char data[100];
} virtual_device;

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

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_ALERT "Nitee Driver: read()\n");
	ret = copy_to_user(buf,virtual_device.data,len);
	return ret;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_ALERT "Nitee Driver: write()\n");
	ret = copy_from_user(virtual_device.data,buf,len);
	return len;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {

	case TIMER_START:
		printk(KERN_ALERT "Nitee IOCTL timer started for 5 secs (%ld)",jiffies);
		timer_setup(&my_timer,timer_callback,0); // setup timer to call the timer call back function
		printk(KERN_ALERT"Nitee set up timer to fire in (%ld)",jiffies);
		retval = mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT)); // Timer started for 5 seconds 
		if (retval)
			printk(KERN_ALERT "Nitee timer firing failed");
		printk(KERN_ALERT "Nitee timer expired after completing 5 secs");
		break;
	case TIMER_STOP:
		printk(KERN_ALERT "Nitee IOCTL timer stopped");
		del_timer(&my_timer);
		printk(KERN_ALERT "Nitee timer stopped after (%ld)", jiffies);
		break;
	case SCHEDULE:
		queue_delayed_work(wq, &my_work, msecs_to_jiffies(5));
		printk(KERN_ALERT "Nitee Scheduled the workqueue to start after 5 secs");
		break;
	}
	return 0;
}

static struct file_operations first_fops =
{
	.owner   	= THIS_MODULE,
	.open    	= my_open,
	.release 	= my_close,
	.unlocked_ioctl = my_ioctl,
	.read    	= my_read,
	.write   	= my_write,
};
  
static int __init first_char_driver_init(void) /* Constructor */
{
	static struct work_data *my_data;
	printk("Nitee Work queue module init: %s %d\n",__FUNCTION__, __LINE__);
	wq = create_singlethread_workqueue("my_single_thread");
	my_data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
	my_data->the_data = 34;
	INIT_WORK(&my_data->my_work, work_handler);
//	queue_delayed_work(wq, &my_work, msecs_to_jiffies(5));

	printk(KERN_INFO "Nitee : first Char Driver registered");

	if (alloc_chrdev_region(&first, 0, 3, "firstChar") < 0) {
		return -1;
	}
	if ((cl = class_create(THIS_MODULE, "firstClass")) == NULL) {
		unregister_chrdev_region(first, 1);
		return -1;
	}
	if (device_create(cl, NULL, first, NULL, "myfirsttimer") == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	cdev_init(&c_dev, &first_fops);
	c_dev.owner = THIS_MODULE;
	c_dev.ops = &first_fops;
	if (cdev_add(&c_dev, first, 1) == -1) {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}


	return 0;
}
 
static void __exit first_char_driver_exit(void) /* Destructor */
{
	flush_workqueue(wq);
        destroy_workqueue(wq);
        printk("Nitee Work queue module exit: %s %d\n",__FUNCTION__, __LINE__);
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
