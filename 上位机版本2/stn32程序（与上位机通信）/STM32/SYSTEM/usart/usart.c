#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>
#define Max_BUFF_Len 40
//串口1队列定义
char Uart1_Buffer[Max_BUFF_Len];
int Uart1_Rx=0;

//串口2队列定义
char Uart2_Buffer[Max_BUFF_Len];
int Uart2_Rx=0;
int i = 0;
#define SUCCESS 0
uint16_t  a_co2=0, a_ligh=0, a_temp=0, a_rh=0;
uint16_t  b_co2=0, b_ligh=0, b_temp=0, b_rh=0;
uint16_t thmax=0, thmin=0,state=0;
char cRes1[6],cRes2[6],cRes3[6],cRes4[6];
char cRes5[6],cRes6[6],cRes7[6],cRes8[6];

float WP,WI,WD,VP,VI,VD,LP,LI,LD,JP,JI,JD,HP,HI,HD;
int state0;//勾选计数

//串口1初始化
void USART1_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStruct;   //定义串口结构体变量
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);   //使能GPIOC的时钟
    //USART1_TX   GPIOA.9
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;   //配置TX引脚
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //配置PA9为复用推挽输出
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA9速率
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
    //USART1_RX      GPIOA.10
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;   //配置RX引脚
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //配置PA10为浮空输入
    //  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA10速率
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //发送接收模式
    USART_InitStruct.USART_Parity=USART_Parity_No;   //无奇偶校验
    USART_InitStruct.USART_BaudRate=bound;   //波特率
    USART_InitStruct.USART_StopBits=USART_StopBits_1;   //停止位1位
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //字长8位
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //无硬件数据流控制
    USART_Init(USART1,&USART_InitStruct);   //串口初始化函数
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //开启串口接收中断
    USART_Cmd(USART1,ENABLE);   //使能USART1
}

//串口2初始化
void USART2_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStruct;   //定义串口结构体变量
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA和USART2时钟
    //USART1_TX   GPIOA.2
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;   //配置TX引脚
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //配置PA9为复用推挽输出
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA9速率
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
    //USART1_RX      GPIOA.3
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;   //配置RX引脚
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //配置PA10为浮空输入
    // GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA10速率
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //发送接收模式
    USART_InitStruct.USART_Parity=USART_Parity_No;   //无奇偶校验
    USART_InitStruct.USART_BaudRate=bound;   //波特率
    USART_InitStruct.USART_StopBits=USART_StopBits_1;   //停止位1位
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //字长8位
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //无硬件数据流控制
    USART_Init(USART2,&USART_InitStruct);   //串口初始化函数
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //开启串口接收中断
    USART_Cmd(USART2,ENABLE);   //使能USART1
}
//串口3初始化
void USART3_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStruct;   //定义串口结构体变量
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启GPIOA和USART1时钟
    //USART3_TX   GPIOB.10
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;   //配置TX引脚
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //配置PA9为复用推挽输出
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA9速率
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
    //USART3_RX      GPIOB.11
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;   //配置RX引脚
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //配置PA10为浮空输入
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA10速率
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //发送接收模式
    USART_InitStruct.USART_Parity=USART_Parity_No;   //无奇偶校验
    USART_InitStruct.USART_BaudRate=bound;   //波特率
    USART_InitStruct.USART_StopBits=USART_StopBits_1;   //停止位1位
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //字长8位
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //无硬件数据流控制
    USART_Init(USART1,&USART_InitStruct);   //串口初始化函数
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);  //开启串口接收中断
    USART_Cmd(USART1,ENABLE);   //使能USART1
}
int fputc(int ch,FILE *f)   //printf重定向函数
{
    USART_SendData(USART1,(uint8_t)ch);   //发送一字节数据
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);   //等待发送完成
    return ch;
}
//串口1发送一帧数据
void USART1_SendOneData(uint8_t SendOneData)
{
    USART_SendData(USART1, SendOneData);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}
}
//串口2发送一帧数据
void USART2_SendOneData(uint8_t SendOneData)
{
    USART_SendData(USART2, SendOneData);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}
}

//串口3发送一帧数据
void USART3_SendOneData(uint8_t SendOneData)
{
    USART_SendData(USART3, SendOneData);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
    {}
}

//串口1发送一列数据
void USART1_SendUnfixedData(uint8_t *Buffer, uint8_t Length)
{
    uint8_t  i;
    for(i=0; i<Length; i++)
    {
        USART1_SendOneData(*Buffer++);
    }
}

//串口2发送一列数据
void USART2_SendUnfixedData(uint8_t *Buffer, uint8_t Length)
{
    uint8_t  i;
    for(i=0; i<Length; i++)
    {
        USART2_SendOneData(*Buffer++);
    }
}

//串口3发送一列数据
void USART3_SendUnfixedData(uint8_t *Buffer, uint8_t Length)
{
    uint8_t  i;
    for(i=0; i<Length; i++)
    {
        USART3_SendOneData(*Buffer++);
    }
}


//串口1中断服务函数
void USART1_IRQHandler(void)
{

    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //中断产生
    {

        Uart1_Buffer[Uart1_Rx] = USART_ReceiveData(USART1);     //接收串口1数据到buff缓冲区
        Uart1_Rx++;
         
			   
			  if(Uart1_Buffer[Uart1_Rx-1] == '#'|| Uart1_Rx == Max_BUFF_Len)    //如果接收到尾标识是s字符（或者等于最大接受数就清空重新接收）
        {
					  if(Uart1_Buffer[0] == 'B')                      //检测到头标识是我们需要的
            {
								state=7;
								test(Uart1_Buffer, cRes1,"B","#");//提取X和#之间的数,判断需要什么波形数据
								state0=atoi(cRes1);//将字符串转为数字	
								delay_ms(6);
								clear_str(cRes1);//清空数据
								Uart1_Rx=0;//清空数据 
            }
						else if(Uart1_Buffer[0] == 'S')    //停止接收串口数据  
            {
								state=0;
								Uart1_Rx=0;//清空数据 
            }
            else
            {
                Uart1_Rx=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
            }
        }
	
      /*接收pid参数设置*/
				
			//P的设置
			 else if(Uart1_Buffer[Uart1_Rx-1] == 'P'|| Uart1_Rx == Max_BUFF_Len)    //如果接收到尾标识是s字符（或者等于最大接受数就清空重新接收）
        {
            if(Uart1_Buffer[0] == 'W')                      //检测到头标识是我们需要的
            {
								state=1;
								test(Uart1_Buffer, cRes1,"W","P");
								WP=atof(cRes1);//将字符串转为数字	
								delay_ms(6);
								clear_str(cRes1);//清空数据
								Uart1_Rx=0;//清空数据 
            }
						else if(Uart1_Buffer[0] == 'V')                      //检测到头标识是我们需要的
            {
								state=2;
								test(Uart1_Buffer, cRes1,"V","P");
								VP=atof(cRes1);//将字符串转为数字	
								delay_ms(6);
								clear_str(cRes1);//清空数据
								Uart1_Rx=0;//清空数据 
            }
            else
            {
                Uart1_Rx=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
            }	
        }		

			//I的设置
			 else if(Uart1_Buffer[Uart1_Rx-1] == 'I'|| Uart1_Rx == Max_BUFF_Len)    //如果接收到尾标识是s字符（或者等于最大接受数就清空重新接收）
        {
            if(Uart1_Buffer[0] == 'W')                      //检测到头标识是我们需要的
            {
								state=3;
								test(Uart1_Buffer, cRes1,"W","I");
								WI=atof(cRes1);//将字符串转为数字	
								delay_ms(6);
								clear_str(cRes1);//清空数据
								Uart1_Rx=0;//清空数据 
            }
						else if(Uart1_Buffer[0] == 'V')                      //检测到头标识是我们需要的
            {
								state=4;
								test(Uart1_Buffer, cRes1,"V","I");
								VI=atof(cRes1);//将字符串转为数字	
								delay_ms(6);
								clear_str(cRes1);//清空数据
								Uart1_Rx=0;//清空数据 
            }
            else
            {
                Uart1_Rx=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
            }	
        }		

			//D的设置
			 else if(Uart1_Buffer[Uart1_Rx-1] == 'D'|| Uart1_Rx == Max_BUFF_Len)    //如果接收到尾标识是s字符（或者等于最大接受数就清空重新接收）
        {
            if(Uart1_Buffer[0] == 'W')                      //检测到头标识是我们需要的
            {
								state=5;
								test(Uart1_Buffer, cRes1,"W","D");
								WD=atof(cRes1);//将字符串转为数字	
								delay_ms(6);
								clear_str(cRes1);//清空数据
								Uart1_Rx=0;//清空数据 
            }
						else if(Uart1_Buffer[0] == 'V')                      //检测到头标识是我们需要的
            {
								state=6;
								test(Uart1_Buffer, cRes1,"V","D");
								VD=atof(cRes1);//将字符串转为数字	
								delay_ms(6);
								clear_str(cRes1);//清空数据
								Uart1_Rx=0;//清空数据 
            }
            else
            {
                Uart1_Rx=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
            }	
        }					

			USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志
    }
		     
    else if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // 发送中断 ?USART_GetITStatus
    {
        USART_ClearITPendingBit(USART1, USART_IT_TXE); // clear interrupt ?清除中断预处理位
    }
}

/*C获取字符串中两个字符之间的的数据*/
int test(char *pcBuf, char *pcRes,char bin[1],char end[1])
{
    char *pcBegin = NULL;
    char *pcEnd = NULL;

    pcBegin = strstr(pcBuf, bin);
    pcEnd = strstr(pcBuf, end);

    if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
    {
        printf("Mail name not found!\n");
    }
    else
    {
        pcBegin += strlen(bin);
        memcpy(pcRes, pcBegin, pcEnd-pcBegin);
    }

    return SUCCESS;
}
/*清除指针数据*/
void  clear_str(char *pcRes)
{
  for (i = 0; i < 6; i++ )
  {
    pcRes[i] = 0;
  }
}

