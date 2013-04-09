/*
 *led.c mini2440led驱动
 */
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>
#include<linux/device.h>

#include <asm/io.h>//io映射头文件

#include "led.h"
/*存放寄存器绝对地址*/
#define GPBCON	0X56000010
#define GPBDAT	0X56000014
#define GPBUP	0X56000018
/*存放寄存器相对地址*/
unsigned volatile short	*rGPBCON;
unsigned volatile char	*rGPBDAT;
unsigned volatile char	*rGPBUP;

dev_t led_num;//存放设备号
struct cdev *my_led;//创建cdev结构  字符设备结构体
int major=0,minor=0;//主设备号，次设备号
int ret,data=0;
static struct class *led_class;
//static struct class_device *led_class_dev;

/*函数声明*/
void led_init(void);
//初始化led
int led_status(int num);
//获取当前led的状态.参数num为led灯的序号0~3.返回0：亮、1：灭

/***************打开实现函数*********************************/
static int led_open(struct inode *inode,struct file *file)
{
//	led_init();//初始化led，包括内存映射，io初始化
	printk("led device open\n");
	return 0;
}

/***************写入实现函数*********************************/
static ssize_t led_write(struct file *filp, const char __user *userbuf,
		size_t count, loff_t *off)
{
	if((ret = copy_from_user((void *)&data,userbuf,sizeof(int))) > 0){
		printk("%d byte fail to write\n",ret);
	}else{
	printk("Write Data %d\n",data);
	}
	return sizeof(int);
}

/***************读取实现函数*********************************/
static ssize_t led_read(struct file *filp, char __user *userbuf,
		size_t count, loff_t *off)
{
	int num = led_status(0);
	if((ret = copy_to_user((void *)userbuf,&num,sizeof(int))) > 0){
		printk("%d byte fail to read\n",ret);
	}else{
	printk("Read Data %d\n",num);
	}
	return sizeof(int);
}

/***************关闭实现函数*********************************/
static int led_release(struct inode *inode,struct file *file)
{
	printk("led device close\n");
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
			setled(cmd,arg);//进入设置led状态函数
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
	.read  = led_read,
	.write = led_write,
	.ioctl = led_ioctl,
	.release = led_release,
};
/**__init函数**/
static int __init dev_init(void)
{
	int result;
	result = alloc_chrdev_region(&led_num,0,1,"ledtest");//动态申请设备号
	if(result<0){
		printk(KERN_WARNING "led:can't get major");	
		return result;//返回错误值
	}

	/*字符设备的注册*/
	my_led=cdev_alloc();//分配空间给my_cdev
	cdev_init(my_led,&myled_fops);//初始化已分配到的结构
	my_led->ops=&myled_fops;//指定file operations
	my_led->owner=THIS_MODULE;//指定模块拥有者
	if(cdev_add(my_led,led_num,1))//将cdev添加入内核
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
	unregister_chrdev_region(led_num,1);//删除设备号
	cdev_del(my_led);//从内核移除设备

	device_destroy(led_class,led_num);//删除设备节点
	class_destroy(led_class);//删除设备类
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Svoday");
