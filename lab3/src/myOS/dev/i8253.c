extern void outb (unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);
void init8253(void)			//初始化PTI: i8253，时钟中断的频率定为100HZ，分频参数为11932
{
	// TODO 请填写代码
	outb(0x43,0x34);
	outb(0x40,11932 & 0xff);
	outb(0x40,11932 >> 8);
	outb(0x21,inb(0x21)&0xFE);
}