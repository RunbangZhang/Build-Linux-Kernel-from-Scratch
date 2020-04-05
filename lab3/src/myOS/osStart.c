extern int myPrintk(int color,const char *format, ...);
extern void clear_screen(void);
extern void myMain(void);
extern void init8253(void);
extern void init8259A(void);
extern void enable_interrupt(void);
extern void disable_interrupt(void);

void osStart(void)
{
	// TODO 请填写代码
	init8259A();
	init8253();
	enable_interrupt();
	
	clear_screen();
	//myPrintk(0x2,"START RUNNING......\n");
	myMain();
	//myPrintk(0x2, "STOP RUNNING......ShutDown\n");
	while(1);
}
