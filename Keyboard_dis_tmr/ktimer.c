#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include<linux/ioport.h>
#include<linux/uaccess.h>
#include<asm/io.h>


 /*unsigned char inb(unsigned short port);
 void outb(unsigned char value, unsigned short port);*/


#define KBD_DATA_REG        0x60    /* I/O port for keyboard data */
#define KBD_CONTROL_REG        0x64 
#define DELAY do { mdelay(1); if (++delay > 10) break; } while(0)

struct timer_list mytimer;
int ticks;
int count = 0;




static void enable_keyboard(void)
{
	long delay = 0;
	//wait till the input buffer is empty
	while (inb(KBD_CONTROL_REG) & 0x02)
		DELAY;
	outb(0xAE, KBD_CONTROL_REG);
}

static void disable_keyboard(void)
{
	long delay = 0;
	//wait till the input buffer is empty
	while (inb(KBD_CONTROL_REG) & 0x02)
		DELAY;
	outb(0xAD, KBD_CONTROL_REG);
}

void mytimer_function(struct timer_list *ptimer)
{
	printk(KERN_INFO " %s : mytimer_function : count = %d\n", THIS_MODULE->name, count);
	count++;
//	mod_timer(&mytimer, jiffies + ticks);
	enable_keyboard();
}


static __init int desd_init(void)
{
	int sec = 20;
	ticks = sec * HZ;	

	timer_setup(&mytimer, mytimer_function, 0);
	mytimer.expires = jiffies + ticks;
	add_timer(&mytimer);

	printk(KERN_INFO " %s : Timer initialisation is done successfully\n", THIS_MODULE->name);
	//keyboard disabled
	disable_keyboard();
	return 0;
}

static __exit void desd_exit(void)
{
	del_timer_sync(&mytimer);
	printk(KERN_INFO " %s : Timer deinitialisation is done successfully\n", THIS_MODULE->name);
}

module_init(desd_init);
module_exit(desd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD @ Sunbeam");
MODULE_DESCRIPTION("This is demo of keyboard disable using kernel timer");














