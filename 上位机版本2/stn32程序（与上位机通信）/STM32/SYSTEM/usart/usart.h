#ifndef   _USART_H
#define   _USART_H
#include "stdint.h"
#include "stm32f10x.h"
extern uint16_t  a_co2,a_ligh, a_temp, a_rh;
extern uint16_t b_co2, b_ligh, b_temp, b_rh;
extern uint16_t thmax, thmin;
extern uint16_t state;
extern  float WP,WI,WD,VP,VI,VD,LP,LI,LD,JP,JI,JD,HP,HI,HD;
#define Max_BUFF_Len 40
//串口1队列定义
extern char Uart1_Buffer[Max_BUFF_Len],str[1];
extern int Uart1_Rx;
extern int state0;//勾选计数
//串口2队列定义
extern char Uart2_Buffer[Max_BUFF_Len];
extern  int Uart2_Rx;

void USART1_SendOneData(uint8_t SendOneData); //串口1发送一帧数据
void USART2_SendOneData(uint8_t SendOneData);
void USART3_SendOneData(uint8_t SendOneData);
void USART1_SendUnfixedData(uint8_t *Buffer, uint8_t Length);
void USART2_SendUnfixedData(uint8_t *Buffer, uint8_t Length);
void USART3_SendUnfixedData(uint8_t *Buffer, uint8_t Length);


#define USART_REC_LEN  200  //定义最大接收字节数 200
#define EN_USART1_RX  1 //使能（1）/禁止（0）串口1接收
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART1_init(uint32_t bound);
void USART2_init(uint32_t bound);
void USART3_init(uint32_t bound);
int test(char *pcBuf, char *pcRes,char bin[2],char end[2]);
void  clear_str(char *pcRes);
#endif

