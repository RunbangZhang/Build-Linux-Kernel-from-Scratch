// NOTE: 以下框架仅供参考。可以推翻重写。
#define NULL 0
extern unsigned char uart_get_char(void);
extern int myPrintk(int color,const char *format, ...);
extern int strcmp(char *str1, char *str2);
extern char *strtok(char *str, const char *delim);

// 命令处理函数
int cmd_handler(int, char **);
int help_handler(int, char **);

// 帮助处理函数
void help_help(void);

struct command {
	char *cmd;
	int (*func)(int argc, char *argv[]);
	void (*help_func)(void);
	char *desc;
} cmds[] = {
	{"cmd", cmd_handler, NULL, "list all commands"},
	{"help", help_handler, help_help, "help [cmd]"},
	{"", NULL, NULL, ""}
};


// help 的帮助
void help_help(void)
{
	// TODO
	myPrintk(0x7,"\nUSAGE : help [cmd]\n");
}

// help 命令处理函数
int help_handler(int argc, char *argv[])
{
	// TODO
	help_help();
	if(argc>2) return 1;
	if(argc==2){
		int i=0;
		while(cmds[i].func!=NULL){
			if(strcmp(argv[1], cmds[i].cmd)==0)	{
				myPrintk(0x7,"\n%s\n",cmds[i].desc);
				break;
			}
  			++i;
  		}
  		if(cmds[i].func==NULL)	return 1;
	}
	return 0;
}

// cmd 命令处理函数
int cmd_handler(int argc, char **argv)
{
	// TODO
	if(argc!=1) return 1;
	myPrintk(0x7,"\nlist all registed commands:\n");
	myPrintk(0x7,"command name : description\n");
	int i=0;
  	while(cmds[i].func!=NULL){
  		myPrintk(0x7,"%13s: %s\n",cmds[i].cmd,cmds[i].desc);
  		++i;
   	}
	return 0;
}


void startShell(void)
{
	// TODO
	int pos,i,argc;
	unsigned char single[2];
	unsigned char buf[200];
	const char split[2]=" ";
	char *args[50];
	unsigned char c;
	while(1)
	{
		myPrintk(0x3,"rbzhang >:");
		pos=0;
		while((c=uart_get_char())!='\r')		//读入一行输入到buf[]
		{
			single[0]=c;
			single[1]='\0';
			myPrintk(0x7,single);
			buf[pos++]=c;
		}
		buf[pos]='\0';

		args[0]=strtok(buf,split);
		i=0;

		
		while(args[i]!=NULL){		
			++i;			
			args[i]=strtok(NULL,split);		//分解一行输入，用指针数组args[]指向每个部分
		}


		if (args[0] == NULL) {
   			myPrintk(0x7, "\n");
   		 	continue;
  		}

  		argc=i;
  		i=0;
  		while(cmds[i].func!=NULL){
  			if (strcmp(args[0],cmds[i].cmd) == 0) {
   			if(1==(*cmds[i].func)(argc,args))myPrintk(0x7, "\nError arguments!\n");
   			break;
   			}
   			i++;
 		}

 		if(cmds[i].func==NULL)
			myPrintk(0x7, "\nUnknown command:%s\n",args[0]);
			
	}
}