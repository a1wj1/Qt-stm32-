#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>
#define Max_BUFF_Len 40
//����1���ж���
char Uart1_Buffer[Max_BUFF_Len];
int Uart1_Rx=0;

//����2���ж���
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
int state0;//��ѡ����

//����1��ʼ��
void USART1_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;   //����GPIO�ṹ�����
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStruct;   //���崮�ڽṹ�����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);   //ʹ��GPIOC��ʱ��
    //USART1_TX   GPIOA.9
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;   //����TX����
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //����PA9Ϊ�����������
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA9����
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
    //USART1_RX      GPIOA.10
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;   //����RX����
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //����PA10Ϊ��������
    //  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA10����
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
    USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //���ͽ���ģʽ
    USART_InitStruct.USART_Parity=USART_Parity_No;   //����żУ��
    USART_InitStruct.USART_BaudRate=bound;   //������
    USART_InitStruct.USART_StopBits=USART_StopBits_1;   //ֹͣλ1λ
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //�ֳ�8λ
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //��Ӳ������������
    USART_Init(USART1,&USART_InitStruct);   //���ڳ�ʼ������
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //�������ڽ����ж�
    USART_Cmd(USART1,ENABLE);   //ʹ��USART1
}

//����2��ʼ��
void USART2_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;   //����GPIO�ṹ�����
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStruct;   //���崮�ڽṹ�����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����GPIOA��USART2ʱ��
    //USART1_TX   GPIOA.2
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;   //����TX����
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //����PA9Ϊ�����������
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA9����
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
    //USART1_RX      GPIOA.3
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;   //����RX����
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //����PA10Ϊ��������
    // GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA10����
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
    //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
    USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //���ͽ���ģʽ
    USART_InitStruct.USART_Parity=USART_Parity_No;   //����żУ��
    USART_InitStruct.USART_BaudRate=bound;   //������
    USART_InitStruct.USART_StopBits=USART_StopBits_1;   //ֹͣλ1λ
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //�ֳ�8λ
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //��Ӳ������������
    USART_Init(USART2,&USART_InitStruct);   //���ڳ�ʼ������
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //�������ڽ����ж�
    USART_Cmd(USART2,ENABLE);   //ʹ��USART1
}
//����3��ʼ��
void USART3_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;   //����GPIO�ṹ�����
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStruct;   //���崮�ڽṹ�����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����GPIOA��USART1ʱ��
    //USART3_TX   GPIOB.10
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;   //����TX����
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //����PA9Ϊ�����������
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA9����
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
    //USART3_RX      GPIOB.11
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;   //����RX����
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //����PA10Ϊ��������
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA10����
    GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
    USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //���ͽ���ģʽ
    USART_InitStruct.USART_Parity=USART_Parity_No;   //����żУ��
    USART_InitStruct.USART_BaudRate=bound;   //������
    USART_InitStruct.USART_StopBits=USART_StopBits_1;   //ֹͣλ1λ
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //�ֳ�8λ
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //��Ӳ������������
    USART_Init(USART1,&USART_InitStruct);   //���ڳ�ʼ������
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);  //�������ڽ����ж�
    USART_Cmd(USART1,ENABLE);   //ʹ��USART1
}
int fputc(int ch,FILE *f)   //printf�ض�����
{
    USART_SendData(USART1,(uint8_t)ch);   //����һ�ֽ�����
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);   //�ȴ��������
    return ch;
}
//����1����һ֡����
void USART1_SendOneData(uint8_t SendOneData)
{
    USART_SendData(USART1, SendOneData);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}
}
//����2����һ֡����
void USART2_SendOneData(uint8_t SendOneData)
{
    USART_SendData(USART2, SendOneData);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}
}

//����3����һ֡����
void USART3_SendOneData(uint8_t SendOneData)
{
    USART_SendData(USART3, SendOneData);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
    {}
}

//����1����һ������
void USART1_SendUnfixedData(uint8_t *Buffer, uint8_t Length)
{
    uint8_t  i;
    for(i=0; i<Length; i++)
    {
        USART1_SendOneData(*Buffer++);
    }
}

//����2����һ������
void USART2_SendUnfixedData(uint8_t *Buffer, uint8_t Length)
{
    uint8_t  i;
    for(i=0; i<Length; i++)
    {
        USART2_SendOneData(*Buffer++);
    }
}

//����3����һ������
void USART3_SendUnfixedData(uint8_t *Buffer, uint8_t Length)
{
    uint8_t  i;
    for(i=0; i<Length; i++)
    {
        USART3_SendOneData(*Buffer++);
    }
}


//����1�жϷ�����
void USART1_IRQHandler(void)
{

    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //�жϲ���
    {

        Uart1_Buffer[Uart1_Rx] = USART_ReceiveData(USART1);     //���մ���1���ݵ�buff������
        Uart1_Rx++;
         
			   
			  if(Uart1_Buffer[Uart1_Rx-1] == '#'|| Uart1_Rx == Max_BUFF_Len)    //������յ�β��ʶ��s�ַ������ߵ�������������������½��գ�
        {
					  if(Uart1_Buffer[0] == 'B')                      //��⵽ͷ��ʶ��������Ҫ��
            {
								state=7;
								test(Uart1_Buffer, cRes1,"B","#");//��ȡX��#֮�����,�ж���Ҫʲô��������
								state0=atoi(cRes1);//���ַ���תΪ����	
								delay_ms(6);
								clear_str(cRes1);//�������
								Uart1_Rx=0;//������� 
            }
						else if(Uart1_Buffer[0] == 'S')    //ֹͣ���մ�������  
            {
								state=0;
								Uart1_Rx=0;//������� 
            }
            else
            {
                Uart1_Rx=0;                                   //����������Ҫ�����ݻ��ߴﵽ����������ʼ���½���
            }
        }
	
      /*����pid��������*/
				
			//P������
			 else if(Uart1_Buffer[Uart1_Rx-1] == 'P'|| Uart1_Rx == Max_BUFF_Len)    //������յ�β��ʶ��s�ַ������ߵ�������������������½��գ�
        {
            if(Uart1_Buffer[0] == 'W')                      //��⵽ͷ��ʶ��������Ҫ��
            {
								state=1;
								test(Uart1_Buffer, cRes1,"W","P");
								WP=atof(cRes1);//���ַ���תΪ����	
								delay_ms(6);
								clear_str(cRes1);//�������
								Uart1_Rx=0;//������� 
            }
						else if(Uart1_Buffer[0] == 'V')                      //��⵽ͷ��ʶ��������Ҫ��
            {
								state=2;
								test(Uart1_Buffer, cRes1,"V","P");
								VP=atof(cRes1);//���ַ���תΪ����	
								delay_ms(6);
								clear_str(cRes1);//�������
								Uart1_Rx=0;//������� 
            }
            else
            {
                Uart1_Rx=0;                                   //����������Ҫ�����ݻ��ߴﵽ����������ʼ���½���
            }	
        }		

			//I������
			 else if(Uart1_Buffer[Uart1_Rx-1] == 'I'|| Uart1_Rx == Max_BUFF_Len)    //������յ�β��ʶ��s�ַ������ߵ�������������������½��գ�
        {
            if(Uart1_Buffer[0] == 'W')                      //��⵽ͷ��ʶ��������Ҫ��
            {
								state=3;
								test(Uart1_Buffer, cRes1,"W","I");
								WI=atof(cRes1);//���ַ���תΪ����	
								delay_ms(6);
								clear_str(cRes1);//�������
								Uart1_Rx=0;//������� 
            }
						else if(Uart1_Buffer[0] == 'V')                      //��⵽ͷ��ʶ��������Ҫ��
            {
								state=4;
								test(Uart1_Buffer, cRes1,"V","I");
								VI=atof(cRes1);//���ַ���תΪ����	
								delay_ms(6);
								clear_str(cRes1);//�������
								Uart1_Rx=0;//������� 
            }
            else
            {
                Uart1_Rx=0;                                   //����������Ҫ�����ݻ��ߴﵽ����������ʼ���½���
            }	
        }		

			//D������
			 else if(Uart1_Buffer[Uart1_Rx-1] == 'D'|| Uart1_Rx == Max_BUFF_Len)    //������յ�β��ʶ��s�ַ������ߵ�������������������½��գ�
        {
            if(Uart1_Buffer[0] == 'W')                      //��⵽ͷ��ʶ��������Ҫ��
            {
								state=5;
								test(Uart1_Buffer, cRes1,"W","D");
								WD=atof(cRes1);//���ַ���תΪ����	
								delay_ms(6);
								clear_str(cRes1);//�������
								Uart1_Rx=0;//������� 
            }
						else if(Uart1_Buffer[0] == 'V')                      //��⵽ͷ��ʶ��������Ҫ��
            {
								state=6;
								test(Uart1_Buffer, cRes1,"V","D");
								VD=atof(cRes1);//���ַ���תΪ����	
								delay_ms(6);
								clear_str(cRes1);//�������
								Uart1_Rx=0;//������� 
            }
            else
            {
                Uart1_Rx=0;                                   //����������Ҫ�����ݻ��ߴﵽ����������ʼ���½���
            }	
        }					

			USART_ClearITPendingBit(USART1,USART_IT_RXNE); //����жϱ�־
    }
		     
    else if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // �����ж� ?USART_GetITStatus
    {
        USART_ClearITPendingBit(USART1, USART_IT_TXE); // clear interrupt ?����ж�Ԥ����λ
    }
}

/*C��ȡ�ַ����������ַ�֮��ĵ�����*/
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
/*���ָ������*/
void  clear_str(char *pcRes)
{
  for (i = 0; i < 6; i++ )
  {
    pcRes[i] = 0;
  }
}

