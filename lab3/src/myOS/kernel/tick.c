extern void maybeUpdateWallClock(void);
int count=0;	//维护tick

void tick(void)			//处理周期性时钟中断
{
	// TODO 请填写代码
	maybeUpdateWallClock();
	++count;
}
