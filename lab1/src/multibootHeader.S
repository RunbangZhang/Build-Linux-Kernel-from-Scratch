.globl start       #声明入口符号

.section .multiboot_header	#自定义一个Header段
        .long 0x1BADB002           	 #magic
        .long 0x00                   #flag
        .long - (0x1BADB002 + 0x00)  #checknum
.code32

.data 		#定义一个数据段，用来声明待输出的字符串
	msg1:
	.ascii "helloworld, pb16071259_zhangrunbang"
	msg2:
	.ascii "HELLOWORLD, PB16071259_ZHANGRUNBANG"

.text    	#代码段
	start:  			#程序的入口点
	movl $0,%edi		#对edi赋初值为0		
	movl $35,%ecx		#对ecx赋初值为35
	movw $0x3F8, %dx	#端口地址传给dx

	loop1:				#进入循环
	movb msg1(,%edi,1), %al		#msg1的字符逐个传给al
	movb %al,0xB8000(,%edi,2)	#字符的值传到VGA显存
	movb $0x2f,0xB8001(,%edi,2)	#字符样式传到VGA显存
	movb msg2(,%edi,1), %al		#msg2的字符逐个传给al
	outb %al, %dx		#在端口写入al
	incl %edi			#edi加1
	loop loop1			#跳转回loop1，除非ecx为0

	nop					#空指令控制时间
	nop
	hlt					#停止程序运行

	movl $1,%eax
	movl $4,%ebx
	int $0*80			#退出

