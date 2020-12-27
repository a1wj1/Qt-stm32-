//              ˵��:
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              ----------------------------------------------------------------

#include "delay.h"
#include "sys.h"
#include "bmp.h"
#include "timer.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
volatile u32 time; // ms ��ʱ����
uint16_t  ReceiveData;
uint16_t temp;

float vol;

//����ѡ��	:senddata��send_data
void senddata(float Z,float X)
{
    printf("Z#%f#X#%f#\r\n",Z,X);

}

//�ֱ������������ѹ�����أ�ʵ��λ�ú�����λ��
void send_data(float I,float V,float T,float actual_threa,float threa_ref)
{
        
		if(state==7)
			{
				switch (state0)
				{
									
					case 11000:{//��������͵�ѹ����
						 delay_ms(100);
						 senddata(I,V);
						
					}break;
	
					case 10100:{//������������ز���
						 delay_ms(100);
						 senddata(I,T);

					}break;
				
					case 10010:{//���������ʵ��λ��	
	          delay_ms(100);						
						senddata(I,actual_threa);	
					
					}break;
					case 10001:{//�������������λ��	
					  delay_ms(100);	
						senddata(I,threa_ref);	
					
					}break;
					
					case 1100:{//�����ѹ�����ز���		
			      delay_ms(100);	
						senddata(V,T);	
						
					}break;
	
					case 1010:{//�����ѹ��ʵ��λ��
	          delay_ms(100);							
						senddata(V,actual_threa);	
						
					}break;
				
					case 1001:{//�����ѹ������λ��	
						delay_ms(100);	
						senddata(V,threa_ref);	
					
					}break;

					case 110:{//������غ�ʵ��λ��
	          delay_ms(100);							
						senddata(T,actual_threa);	
					
					}break;
	
					case 101:{//������غ�����λ��
	          delay_ms(100);							
						senddata(T,threa_ref);	
		
					}break;
				
					case 11:{//���ʵ�ʺ�����λ��
	          delay_ms(100);							
						senddata(actual_threa,threa_ref);
					
					}break;										
					
					default:break;			
					
				}
			}	
}



void pid_set()
{
	 //������λ���������	
    //pid����		
		if(state==1)
			{
				 printf("%f",WP);	
				 state=0;				
			}
		else	if(state==2)
			{
				 printf("%f",VP);	
				 state=0;				
			}
		else	if(state==3)
			{
				 printf("%f",WI);	
				 state=0;				
			}
		else	if(state==4)
			{
				 printf("%f",VI);	
				 state=0;				
			}
		else	if(state==5)
			{
				 printf("%f",WD);	
				 state=0;				
			}
		else	if(state==6)
			{
				 printf("%f",VD);	
				 state=0;				
			}
		else	if(state==0)
			{	
				 state=0;		//����������		
			}

}

int main (void)
{
	    delay_init();        //��ʱ������ʼ��
      SystemInit(); /* ����ϵͳʱ��Ϊ 72M */
      USART1_init(115200);
      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	while(1)
	{		  

			pid_set();
			send_data(10.1,20.1,30.1,40.1,50.1);				
			
	}
    	 
}




