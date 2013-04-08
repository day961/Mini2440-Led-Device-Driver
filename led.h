#define GPBCON	0X56000010
#define GPBDAT	0X56000014
#define GPBUP	0X56000018

extern unsigned volatile short	*rGPBCON;
extern unsigned volatile char   *rGPBDAT;
extern unsigned volatile char	*rGPBUP;

int setled(unsigned int cmd,unsigned long arg);
void led_init(void);

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
}

