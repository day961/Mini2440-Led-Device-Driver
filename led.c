/*
 *led.c mini2440led驱动
 */
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/ioctl.h>
#include<linux/device.h>

#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/gpio.h>

#include <asm/io.h>

#include "led.h"
/*存放寄存器绝对地址*/
#define GPBCON	0X56000010
#define GPBDAT	0X56000014
#define GPBUP	0X56000018
/*存放寄存器相对地址*/
unsigned short	*rGPBCON;
unsigned char	*rGPBDAT;
unsigned char	*rGPBUP;

dev_t led_num;//存放设备号
struct cdev *my_led;//创建cdev结构  字符设备结构体
int major=0,minor=0;
static struct class *led_class;
//static struct class_device *led_class_dev;


/***************打开实现函数*********************************/
static int led_open(struct inode *inode,struct file *file)
{
	printk("led device open\n");
	return 0;
}
/**************IOctrl实现函数******************************/
/*
cmd:led控制命令 0关1开
arg:led选择	led0~3
 */
static int led_ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
		case 0:
		case 1:
	/*		if(arg > 4)
			{       
				return -EINVAL;
			}//检查led号
				switch(arg)
				{
					case 0:
					value_dat |= 0x2;
					iowrite16(value_dat,rGPBDAT);
					return 0;
					break;
					case 1:
					value_dat &= (~0x1E0);
					iowrite16(value_dat,rGPBDAT);
					return 0;
					break;
				}
	*/
			setled(cmd,arg);
			break;
			return 0;
		default: 
			return -EINVAL;
	}
	return 0;
}

/***************file operations结构*********************/
static struct file_operations myled_fops = 
{
	.owner = THIS_MODULE, 
	.open  = led_open,
	.ioctl = led_ioctl,
};
/*******************************************************/

void led_init(void)
{
	unsigned int 	value_con;	//别用short!!!!!
	unsigned int	value_up;
	unsigned int	value_dat;
				
	rGPBCON	= ioremap(GPBCON , 4);
	rGPBDAT	= ioremap(GPBDAT , 2);
	rGPBUP	= ioremap(GPBUP  , 2);

	value_con = ioread32(rGPBCON);//读取内存
	value_con = (value_con&(~(0xff<<10)))|(0x15400); //0x3fc00 = 0011,1111,1100,0000,0000. 0x15400 = 0001,0101,0100,0000,0000
	iowrite32(value_con,rGPBCON);//写回内存 记得写够32位！！被坑了！！

	value_up = ioread16(rGPBUP);
	value_up |= 0x1e0;	//0x1e0 = 0001,1110,0000
	iowrite16(value_up,rGPBUP);
	
	value_dat = ioread16(rGPBDAT);
	value_dat |= 0x1e0;	//全部熄灭
	iowrite16(value_dat,rGPBDAT);

	printk("led init complete\n");

//	s3c2410_gpio_cfgpin(S3C2410_GPB(8),S3C2410_GPIO_OUTPUT);
//	s3c2410_gpio_setpin(S3C2410_GPB(8),0);
}

/**__init函数**/
static int __init dev_init(void)
{
	int result;
	result = alloc_chrdev_region(&led_num,0,1,"ledtest");//动态申请设备号
	if(result<0){
		printk(KERN_WARNING "led:can't get major");	
		return result;//检查是否申请成功
	}

	/*字符设备的注册*/
	my_led=cdev_alloc();//分配空间给my_cdev
	cdev_init(my_led,&myled_fops);//初始化已分配到的结构
	my_led->ops=&myled_fops;//指定file operations
	my_led->owner=THIS_MODULE;
	if(cdev_add(my_led,led_num,1))//将cdev添加如内核
		printk(KERN_NOTICE"Someting wrong when adding cdev\n");
	else
		printk("Success adding cdev\n");


	led_init();//初始化led，包括内存映射，io初始化


	/*用于在/dev创建设备节点*/
	led_class = class_create(THIS_MODULE,"ledtest");//创建设备类
	device_create(led_class,NULL,led_num,NULL,"ledtest");//创建设备节点
	return 0;
}


/************************************************/
static void __exit dev_exit(void)
{
	printk("Unloading cdev \n");
	unregister_chrdev_region(led_num,1);//删除中断号
	cdev_del(my_led);//从内核移除设备
	device_destroy(led_class,led_num);//删除设备节点
	class_destroy(led_class);//删除设备类
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Svoday");

