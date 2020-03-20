//本文件实现vga的相关功能，清屏和屏幕输出，clear_screen和append2screen必须按照如下实现，可以增加其他函数供clear_screen和append2screen调用

extern void disable_interrupt(void);
extern void enable_interrupt(void);

extern void outb (unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);

int cursor_row, cursor_col;      //光标

void clear_screen(void) {			
	int row, col;
	unsigned short *ptr = (unsigned short *)0xb8000;
	for(row = 0; row < 25; row++) {
	    for (col = 0; col < 80; col++) {
		(*ptr++) = 0;
	    }
	}
}

void put_char(char c, char color) {

	int i;
    unsigned char *ptr = (unsigned char *)0xb8000;
    unsigned int pos;
    
    if(c == '\n'||cursor_col == 80)
    {
        cursor_col = 0;
        cursor_row = (cursor_row + 1);
        if(cursor_row==25)			//即将执行滚屏操作
        {
        	for(i = 0; i < 24*80*2; i++) 		
        		ptr[i]=ptr[i+80*2];
        	for(i = 24*80*2; i < 25*80*2; i++) 
        		ptr[i]=0;
        	cursor_row=24;
		}
        
    }

    if(c!='\n')
    {
        ptr[(cursor_row * 80 + cursor_col) * 2] = c;			//写入字符值
        ptr[(cursor_row * 80 + cursor_col) * 2 + 1] = color;  		//写入字符颜色
        cursor_col ++;	
    }

    ptr[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x7;  //设置光标颜色
    pos=cursor_row*80+cursor_col;						//设置光标位置
    outb(0x3d4,14);
    outb(0x3d5,(pos>>8) & 0xff);
    outb(0x3d4,15);
    outb(0x3d5,pos & 0xff);
}

void append2screen(char *str,int color){ 
	char *ptr = str;
    char c;
    
    c = *ptr;
    while (c!='\0'){
        put_char(c, color);     //逐个字符调用put_char()实现VGA输出
        c = *(++ptr);
    }
}
