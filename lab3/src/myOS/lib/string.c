#define NULL 0
int strlen(char *str)
{
    int n=0;
    while(*str++) n++;
    return n;
}

int strcpy(char *src, char *dst){
    int n=0;
    while(*src) {
        *dst++ = *src++;
        n++;
    }
    return n;
}

int strcmp(char *str1, char *str2){
    char c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
        if (c1 != c2) return (c1 > c2)?1:-1;
        if (c1 == '\0') break;
    } while(1);
    return 0;
}

int strncmp( char *str1,  char *str2,  int n){
     char c1, c2;    

    while(n){
        c1 = *str1++;
        c2 = *str2++;        
        if (c1 != c2) return (c1 > c2)?1:-1;
        if (c1 == '\0') break;
    };
    return 0;
}

#include<stdio.h>

//该程序的关键点在于定义了一个静态的字符指针,该指针用来记录分割后的字符串的首地址
//传入NULL,则表示继续处理静态指针指向的余下字符串
char *strtok(char *str, const char *delim)
{
        static char *src=NULL;                                         //记下上一次非分隔字符串字符的位置,详见图示
        const char *indelim=delim;                                  //对delim做一个备份
        int flag=1,index=0;                                
    //每一次调用strtok,flag标记都会使得程序只记录下第一个非分隔符的位置,以后出现非分隔符不再处理
        char *temp=NULL;                                       //程序的返回值
 
        if(str==NULL)
        {
          str=src;                                               //若str为NULL则表示该程序继续处理上一次余下的字符串
        }
        for(;*str;str++)
        {
           delim=indelim;
          for(;*delim;delim++)
           {
                  if(*str==*delim)
                  {
                          *str='\0';                    //若找到delim中感兴趣的字符,将该字符置为NULL
                          index=1;                         //用来标记已出现感兴趣字符
                          break;
                  }
           }
          if(*str != '\0' && flag==1)
          {
                  temp=str;                              //只记录下当前第一个非感兴趣字符的位置
                  flag=0;  
           }
          if(*str != '\0' && flag==0 && index==1)
          {
                 src=str;                                   //第二次出现非感兴趣字符的位置(之前一定出现过感兴趣字符)
                 return temp;
  }
        }
        src=str;                              
      //执行该句表明一直未出现过感兴趣字符,或者说在出现了感兴趣的字符后,就没再出现过非感兴趣字符
        return temp;
}



 