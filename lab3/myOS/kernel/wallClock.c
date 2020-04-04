int hh=0, mm=0, ss=0, ms=0;

void setWallClock(int h, int m, int s)
{
	// TODO 请填写代码
}

void getWallClock(int *h, int *m, int *s)
{
        *h = hh;
        *m = mm;
        *s = ss;
}

void setWallClockHook(void (*func)(void)) 
{
	// TODO 请填写代码
}

void maybeUpdateWallClock(void)
{
	// 请填写代码
	if (wallClock_hook) wallClock_hook();
}
