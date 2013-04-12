#define GPBCON	0X56000010
#define GPBDAT	0X56000014
#define GPBUP	0X56000018

/*led掩码*/
#define led0 (1<<5)
#define led1 (1<<6)
#define led2 (1<<7)
#define led3 (1<<8)

extern unsigned volatile short	*rGPBCON;
extern unsigned volatile char   *rGPBDAT;
extern unsigned volatile char	*rGPBUP;

int setled(unsigned int cmd,unsigned long arg);
void led_init(void);


/*led状态处理函数*/
int setled(unsigned int cmd,unsigned long arg)
{
	unsigned int value_dat=0;
	value_dat = ioread16(rGPBDAT);
	if(cmd == 0)//开灯
	{
		switch(arg)
		{
			case 0://led0
				value_dat &=~(1<<5);
				iowrite32(value_dat,rGPBDAT);
				break;
			case 1://led1
				value_dat &=~(1<<6);
				iowrite32(value_dat,rGPBDAT);
				break;
			case 2:
				value_dat &=~(1<<7);
				iowrite32(value_dat,rGPBDAT);
				break;
			case 3:
				value_dat &=~(1<<8);
				iowrite32(value_dat,rGPBDAT);
				break;
			default:
				return -EINVAL;
		}
	}
	else if(cmd == 1)//关灯
	{
		switch(arg)
		{
			case 0://led0
				value_dat |=(1<<5);
				iowrite32(value_dat,rGPBDAT);
				break;
			case 1://led1
				value_dat |=(1<<6);
				iowrite32(value_dat,rGPBDAT);
				break;
			case 2:
				value_dat |=(1<<7);
				iowrite32(value_dat,rGPBDAT);
				break;
			case 3:
				value_dat |=(1<<8);
				iowrite32(value_dat,rGPBDAT);
				break;
			default:
				return -EINVAL;
		}
	}
	return 0;
}
/*
   改变led状态
 */
void led_contrl(char cmd,char arg)
{
	unsigned int value_dat;
	value_dat = ioread16(rGPBDAT);
	
	value_dat =(value_dat&(~(1<<(arg+5))))|(cmd<<(arg+5));	
	iowrite16(value_dat,rGPBDAT);	
}

/*
  led状态读取函数
  返回led灯状态，0：亮、1：灭
  参数num为led序号
 */
int led_status(int num)
{	
	unsigned int value_dat;
	value_dat = ioread16(rGPBDAT);
//	printk("value_dat=%d\n",value_dat);
	if((value_dat&(1<<(num+5))) == 0)//+5 因为led0是从GPB5开始
	{
		return 0;//若判断为亮
	}
	return 1;
}

/*leds设备初始化函数*/
void led_init(void)
{
	unsigned int 	value_con;	//别用short!!!!!
	unsigned int	value_up;
	unsigned int	value_dat;
				
	rGPBCON	= ioremap(GPBCON , 4);
	rGPBDAT	= ioremap(GPBDAT , 2);
	rGPBUP	= ioremap(GPBUP  , 2);

	value_con = ioread32(rGPBCON);//读取内存
	value_con = (value_con&(~(0xff<<10)))|(0x15400); 
/*	0x3fc00 = 0011,1111,1100,0000,0000.
	0x15400 = 0001,0101,0100,0000,0000
	设为输出模式*/
	iowrite32(value_con,rGPBCON);//写回内存 记得写够32位！！被坑了！！

	value_up = ioread16(rGPBUP);
	value_up |= 0x1e0;	//0x1e0 = 0001,1110,0000
	iowrite16(value_up,rGPBUP);
	
	value_dat = ioread16(rGPBDAT);
	value_dat |= 0x1e0;	//全部熄灭
	iowrite16(value_dat,rGPBDAT);

	printk("led init complete\n");
}

