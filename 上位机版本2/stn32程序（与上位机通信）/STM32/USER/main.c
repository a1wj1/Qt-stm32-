//              说明:
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              ----------------------------------------------------------------

#include "delay.h"
#include "sys.h"
#include "bmp.h"
#include "timer.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
volatile u32 time; // ms 计时变量
uint16_t  ReceiveData;
uint16_t temp;

float vol;

//波形选择	:senddata与send_data
void senddata(float Z,float X)
{
    printf("Z#%f#X#%f#\r\n",Z,X);

}

//分别输入电流，电压，力矩，实际位置和理论位置
void send_data(float I,float V,float T,float actual_threa,float threa_ref)
{
        
		if(state==7)
			{
				switch (state0)
				{
									
					case 11000:{//输出电流和电压波形
						 delay_ms(100);
						 senddata(I,V);
						
					}break;
	
					case 10100:{//输出电流和力矩波形
						 delay_ms(100);
						 senddata(I,T);

					}break;
				
					case 10010:{//输出电流和实际位置	
	          delay_ms(100);						
						senddata(I,actual_threa);	
					
					}break;
					case 10001:{//输出电流和理论位置	
					  delay_ms(100);	
						senddata(I,threa_ref);	
					
					}break;
					
					case 1100:{//输出电压和力矩波形		
			      delay_ms(100);	
						senddata(V,T);	
						
					}break;
	
					case 1010:{//输出电压和实际位置
	          delay_ms(100);							
						senddata(V,actual_threa);	
						
					}break;
				
					case 1001:{//输出电压和理论位置	
						delay_ms(100);	
						senddata(V,threa_ref);	
					
					}break;

					case 110:{//输出力矩和实际位置
	          delay_ms(100);							
						senddata(T,actual_threa);	
					
					}break;
	
					case 101:{//输出力矩和理论位置
	          delay_ms(100);							
						senddata(T,threa_ref);	
		
					}break;
				
					case 11:{//输出实际和理论位置
	          delay_ms(100);							
						senddata(actual_threa,threa_ref);
					
					}break;										
					
					default:break;			
					
				}
			}	
}



void pid_set()
{
	 //接收上位机处理语句	
    //pid调参		
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
				 state=0;		//不发送数据		
			}

}

int main (void)
{
	    delay_init();        //延时函数初始化
      SystemInit(); /* 配置系统时钟为 72M */
      USART1_init(115200);
      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	while(1)
	{		  

			pid_set();
			send_data(10.1,20.1,30.1,40.1,50.1);				
			
	}
    	 
}




