unsigned char inb(unsigned short int port_from)			//内嵌式汇编，从端口读入
{
    unsigned char _in_value;
    __asm__ __volatile__ ("inb %w1,%0":"=a"(_in_value):"Nd"(port_from));
    return _in_value;
}

void outb (unsigned short int port_to, unsigned char value)		//内嵌式汇编，端口输出
{
    __asm__ __volatile__ ("outb %b0,%w1"::"a" (value),"Nd" (port_to));
}
