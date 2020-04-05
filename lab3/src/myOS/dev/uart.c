// TODO：请放入 lab2 实现
//调用inb和outb函数，实现下面的uart的三个函数
extern unsigned char inb(unsigned short int port_from);
extern void outb (unsigned short int port_to, unsigned char value);

#define uart_base 0x3F8

void uart_put_char(unsigned char c){        //串口输出单个字符

    if(c == '\n'){				//特殊处理'\n'符
    	outb(uart_base,'\r');
    	outb(uart_base,'\n');
    }
    else{
        outb(uart_base,c);
    }

}

unsigned char uart_get_char(void){         //从串口接受输入字符 
    while(!(inb(uart_base+5)&1));
	return inb(uart_base);
}

void uart_put_chars(char *str){        //串口输出字符串
	char *ptr = str;
    char c;
    
    c = *ptr;
    while (c!='\0'){
        uart_put_char(c);     //逐个字符调用uart_put_char实现串口输出
        c = *(++ptr);
    }
}