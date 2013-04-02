#define GPBDAT  0X56000014
extern unsigned char   *rGPBDAT;
int setled(unsigned int cmd,unsigned long arg);

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
