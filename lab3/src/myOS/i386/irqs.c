void ignoreIntBody(void)            //处理除时钟中断之外的所有其他中断
{
	//put_chars("Unknown interrupt1\0",0x4,24,0);
	unsigned char *ptr = (unsigned char *)0xb8000;
	char *str="Unknown interrupt1\0";
	char c;
	int row=24,col=0;
    c = *str;
    while (c!='\0'){
        ptr[(row * 80 + col) * 2] = c;			
        ptr[(row * 80 + col) * 2 + 1] = 0x4;
        c = *(++str);
        col ++;
    }
}
