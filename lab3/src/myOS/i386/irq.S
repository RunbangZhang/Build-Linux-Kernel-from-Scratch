.text
.code32
	.globl enable_interrupt
	.globl disable_interrupt

enable_interrupt:		#开中断
	sti
	ret
	
disable_interrupt:		#关中断
	cli
	ret