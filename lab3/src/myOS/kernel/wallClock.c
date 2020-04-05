int hh=0, mm=0, ss=0, ms=0;
extern int count;

void setWallClock(int h, int m, int s)		//初始化墙钟
{
	// TODO 请填写代码
	hh=h;
	mm=m;
	ss=s;
}

void getWallClock(int *h, int *m, int *s)		//读取墙钟
{
        *h = hh;
        *m = mm;
        *s = ss;
}

void displayClock()							//右下角显示墙钟
{
	int hour,minute,second;
	getWallClock(&hour,&minute,&second);

	//unsigned char *pos = (unsigned char *)(0xb8000+160);
	int where = 0xb8000 + 25*80*2;
    *(unsigned char *)(where - 2) = (second % 10) + '0';
    *(unsigned char *)(where - 1) = 0x6;
    *(unsigned char *)(where - 4) = (second - second % 10) / 10 + '0';
    *(unsigned char *)(where - 3) = 0x6;

    *(unsigned char *)(where - 6) = ':';
    *(unsigned char *)(where - 5) = 0x6;
    
    *(unsigned char *)(where - 8) = (minute % 10) + '0';
    *(unsigned char *)(where - 7) = 0x6;
    *(unsigned char *)(where - 10) = (minute - minute % 10) / 10 + '0';
    *(unsigned char *)(where - 9) = 0x6;

    *(unsigned char *)(where - 12) = ':';
    *(unsigned char *)(where - 11) = 0x6;

    *(unsigned char *)(where - 14) = (hour % 10) + '0';
    *(unsigned char *)(where - 13) = 0x6;
    *(unsigned char *)(where - 16) = (hour - hour % 10) / 10 + '0';
    *(unsigned char *)(where - 15) = 0x6;

}

void setWallClockHook(void (*func)(void)) 			//设置hook
{
	// TODO 请填写代码
	(*func)();
}

void maybeUpdateWallClock(void)		//根据tick维护墙钟
{
	// 请填写代码
	static int flag = 1;
	if(1==flag)
	{
   	 setWallClock(12,26,38);
   	 flag = 0;
	}

	ms+=10;
	if(ms>=1000) {ms=0;ss++;}
	if(ss>=60) {ss=0;mm++;}
	if(mm>=60) {mm=0;hh++;}
	if(hh>=24) hh=0;

	if(0==count%100) setWallClockHook(displayClock);

	void (*wallClock_hook)()=0;
	if (wallClock_hook) wallClock_hook();
}

