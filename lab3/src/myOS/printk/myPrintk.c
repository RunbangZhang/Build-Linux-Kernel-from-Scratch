// TODO：请放入 lab2 实现
//实现myPrint功能，需要调用到格式化输出的function（vsprintf）
#include <stdarg.h>             //需要提供实现可变参数功能的函数和宏
#include "vsprintf.c"           //需要引用移植的vsprintf()
extern void append2screen(char *str,int color);
extern void uart_put_chars(char *str);
extern int vsprintf(char *buf, const char *fmt, va_list args);

char kBuf[400];  //TODO: fix me
int myPrintk(int color,const char *format, ...){ 
	va_list ap;                
 	va_start(ap,format);
    vsprintf(kBuf,format,ap);    //调用vsprintf处理格式化字符串，结果存储在kBuf[]中
    va_end(ap);
    uart_put_chars(kBuf);          //调用串口输出
    append2screen(kBuf,color);      //调用VGA输出
}

char uBuf[400];  //TODO: fix me
int myPrintf(int color,const char *format, ...){
	va_list ap;
 	va_start(ap,format);
    vsprintf(uBuf,format,ap);   //调用vsprintf处理格式化字符串，结果存储在uBuf[]中
    va_end(ap);
    uart_put_chars(uBuf);          //调用串口输出
    append2screen(uBuf,color);      //调用VGA输出
}