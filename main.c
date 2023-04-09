#include "stm32f446xx.h"
#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "TIMER.h"
#include <stdio.h>
#include<string.h>
#include<stdlib.h>


uint32_t trafficTime=20,time_delay_cnt;//uint32_t
uint32_t TotalTimeCount=0;//uint32_t
int Flag = 0;
static char receive_buffer[50],output_buffer[100],input_buffer[100],str1[50],str2[50],ch,reply[100];
static int i=0,j=0,k=0,R1=10,R2=10,Y1=2,Y2=2,G1=10,G2=10,u1=5,u2=3;
static int Flag6_1=0,Flag1_6=0;
static char log[500][4][50];
void ShowStatus(void);
void AllStatusPrint(void);
void StringParsing(char *str);
void ControlToTraffic(void);//Control Room to Traffic Light
void TrafficToControl(void);//Traffic Light to Control Room

void USART1_IRQHandler(void){
		if((USART1->SR & USART_SR_RXNE) && Flag6_1==1 ){
		while(!(USART1->SR & USART_SR_RXNE));
		receive_buffer[i] = (uint8_t)USART1->DR;
		USART1->SR &= ~(USART_SR_RXNE);
		if(receive_buffer[i]=='\0' || receive_buffer[i]=='\n')
		{
			  //UART_SendString(USART2,recevie);
			  StringParsing(receive_buffer);
		}
  }
	if((USART1->SR & USART_SR_TXE ) && Flag1_6==1){
		USART1->DR = reply[i];
		while(!(USART1->SR & USART_SR_TXE));
		USART1->SR &= ~(USART_SR_TXE);
		USART1->CR1 &= ~(USART_CR1_TXEIE);
	}
}

void USART6_IRQHandler(void){
		 if((USART6->SR & USART_SR_RXNE) && Flag1_6==1){
		 while(!(USART6->SR & USART_SR_RXNE));
		 output_buffer[j] = (uint8_t)USART6->DR;
		 USART6->SR &= ~(USART_SR_RXNE);
			 if(output_buffer[j]=='\n' || output_buffer[j]=='\0')
			 {
					//UART_SendString(USART2,output_buffer);

			 }
		}
		
		if((USART6->SR & USART_SR_TXE) && Flag6_1==1 ){
		USART6->DR = input_buffer[j];
		while(!(USART6->SR & USART_SR_TXE));
		USART6->SR &= ~(USART_SR_TXE);
		USART6->CR1 &= ~(USART_CR1_TXEIE);
	}
}

//Control Room ---->6
//Traffic Light---->1

void ControlToTraffic(void)
{
		i=0;
		j=0;
	  Flag6_1=1;
		Flag1_6=0;
		ms_delay(1000);
		while(i<=strlen(input_buffer))
		{
				USART6->CR1 |= USART_CR1_TXEIE; 
				ms_delay(100);
				i++;j++;
		}
		//recevie[i]='\0';
		//ms_delay(1000);
}
void TrafficToControl(void)
{
		i=0;
		j=0;
	  Flag6_1=0;
		Flag1_6=1;
		ms_delay(1000);
	  output_buffer[0]='\0';
		while(reply[i]!=NULL)
		{
				USART1->CR1 |= USART_CR1_TXEIE; 
				ms_delay(100);
				i++;j++;
		}
		output_buffer[i]='\0';
		UART_SendString(USART2,output_buffer);
}

void USART2_IRQHandler(void) {
	
	 if (USART2->SR & USART_SR_RXNE) 
	 {
			while(!(USART2->SR & USART_SR_RXNE));
			char ch=(uint8_t)USART2->DR;
			input_buffer[k++]= ch;
			USART2->SR &= ~USART_SR_RXNE;
			if(ch=='\0' || ch=='\n')
			{
					input_buffer[k]= '\0';
					Flag=1;
					k=0;
			}
	 }
	 if (USART2->SR & USART_SR_TXE){
		 
        while(!(USART2->SR & USART_SR_TXE));
        USART2->SR &= ~(USART_SR_TXE);
        USART2->CR1 &= ~(USART_CR1_TXEIE);
    }
}

void TIM6_DAC_IRQHandler(void)
{

	  if(TIM6->SR & (TIM_SR_UIF))
		{
				TIM6->SR &=~(TIM_SR_UIF);
				TotalTimeCount++;
				//if((totaltime % traffic)==0)
				ShowStatus();
		}
}

void ShowStatus(void)
{
	  //UART_SendString(USART2,"Here\n");
	  sprintf(reply, "%d",TotalTimeCount);
		strcat(reply," traffic light 1");
		if(GPIO_ReadPin(GPIOC, 0)) //Red1
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOC, 1))// Yellow1
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOB, 0))//Green1
			strcat(reply," ON\n");
		else strcat(reply," OFF\n");
		strcpy(log[TotalTimeCount][0],reply);
		
		
		sprintf(reply, "%d",TotalTimeCount );
		strcat(reply," traffic light 2");
		if(GPIO_ReadPin(GPIOC, 13))//RED2
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOC, 14))//Yellow2
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOC, 15))//Green2
			strcat(reply," ON\n");
		else strcat(reply," OFF\n");
		strcpy(log[TotalTimeCount][1],reply);
		
		
		sprintf(reply, "%d",TotalTimeCount);
		strcat(reply," road north south");
		if(GPIO_ReadPin(GPIOC, 10))//North-South
			strcat(reply,"  heavy traffic\n");
		else strcat(reply,"  light traffic\n");
		strcpy(log[TotalTimeCount][2],reply);
		
		
		sprintf(reply, "%d",TotalTimeCount);
		strcat(reply," road east west");
		if(GPIO_ReadPin(GPIOC, 12))//East-West
			strcat(reply,"  heavy traffic\n");
		else strcat(reply,"  light traffic\n");
		strcpy(log[TotalTimeCount][3],reply);
}
void StringParsing(char *str)
{
	  uint32_t number;
	  sscanf(str,"%s %s %d",str1,str2,&number);;
	  //UART_SendString(USART2,"Successful\n");
    if(strcmp(str2,"light")==0)
    {
				//UART_SendString(USART2,s);
        if(number==1)
        {
            sscanf(str,"%s %s %d %c %c %c %d %d %d %d",str1,str2,&number,&ch,&ch,&ch,&G1,&Y1,&R1,&u1);
        }
        else
        {
            sscanf(str,"%s %s %d %c %c %c %d %d %d %d",str1,str2,&number,&ch,&ch,&ch,&G2,&Y2,&R2,&u2);
        }
    }
		else {
			trafficTime=number;
		  AllStatusPrint();
		}
}


void AllStatusPrint(void)
{
	int index;
	for(index = 2; index >=0 ;index--)
	{
		if((TotalTimeCount - (uint32_t)index*trafficTime)>=0){
			UART_SendString(USART2,log[TotalTimeCount - (uint32_t)index*trafficTime][0]);
			UART_SendString(USART2,log[TotalTimeCount - (uint32_t)index*trafficTime][1]);
			UART_SendString(USART2,log[TotalTimeCount - (uint32_t)index*trafficTime][2]);
			UART_SendString(USART2,log[TotalTimeCount - (uint32_t)index*trafficTime][3]);
		}
		else break;
	}
}
int main(void)
{
		
		//Initialization
		initClock();
		sysInit();
	  USART1_Config();
		USART2_Config();
	  USART6_Config();
		GPIOx_MODER_Set();
		TIM6Config();
	  while(1)
	  {
			//sprintf(send,"%d\n",flag);
			//UART_SendString(USART2,send);
			if(Flag == 1)
			{ 
					ControlToTraffic();
				  Flag=0;
			}
			int RandomNum = rand()%2+1;
			if(RandomNum==1)
			{
					GPIO_WritePin(GPIOA, 7, GPIO_PIN_SET);//Green1
					GPIO_WritePin(GPIOB, 8, GPIO_PIN_SET);//Traffic1
					GPIO_WritePin(GPIOB, 9, GPIO_PIN_SET);//Traffic2
					GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);//Red2
					time_delay_cnt= (G1 + u2)*1000;
					ms_delay((uint32_t)time_delay_cnt);
					GPIO_WritePin(GPIOA, 7, GPIO_PIN_RESET);//Green1
					GPIO_WritePin(GPIOA, 6, GPIO_PIN_SET);//Yellow1
					time_delay_cnt = Y1*1000;
					ms_delay((uint32_t)time_delay_cnt);
				  GPIO_WritePin(GPIOA, 6, GPIO_PIN_RESET);///Yellow1
					GPIO_WritePin(GPIOA, 5, GPIO_PIN_SET);//Red1
					GPIO_WritePin(GPIOB, 9, GPIO_PIN_RESET);//Traffic2
					GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);//Green2
					GPIO_WritePin(GPIOB, 5, GPIO_PIN_RESET);//Red2
					time_delay_cnt= (G2 + u1)*1000;
					ms_delay((uint32_t)time_delay_cnt);
					GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);//Green2
					GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);//Yellow2
					time_delay_cnt = Y2*1000;
					ms_delay((uint32_t)time_delay_cnt);
					GPIO_WritePin(GPIOB, 6, GPIO_PIN_RESET);//Yellow2
					GPIO_WritePin(GPIOA, 5, GPIO_PIN_RESET);//Red1
					GPIO_WritePin(GPIOB, 8, GPIO_PIN_RESET);//Traffic1
					ms_delay(1000);
			}
			else
			{
				  GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);//Green2
					GPIO_WritePin(GPIOB, 8, GPIO_PIN_SET);//Traffic1
					GPIO_WritePin(GPIOB, 9, GPIO_PIN_SET);//Traffic2
					GPIO_WritePin(GPIOA, 5, GPIO_PIN_SET);//Red1
					time_delay_cnt= (G2 + u1)*1000;
					ms_delay((uint32_t)time_delay_cnt);
					GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);//Green2
					GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);//Yellow2
					time_delay_cnt = Y2*1000;
					ms_delay((uint32_t)time_delay_cnt);
				  GPIO_WritePin(GPIOB, 6, GPIO_PIN_RESET);///Yellow1
					GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);//Red2
					GPIO_WritePin(GPIOB, 9, GPIO_PIN_RESET);//Traffic2
					GPIO_WritePin(GPIOA, 7, GPIO_PIN_SET);//Green1
					GPIO_WritePin(GPIOA, 5, GPIO_PIN_RESET);//Red1
					time_delay_cnt= (G1 + u2)*1000;
					ms_delay((uint32_t)time_delay_cnt);
					GPIO_WritePin(GPIOA, 7, GPIO_PIN_RESET);//Green1
					GPIO_WritePin(GPIOA, 6, GPIO_PIN_SET);//Yellow1
					time_delay_cnt = Y1*1000;
					ms_delay((uint32_t)time_delay_cnt);
					GPIO_WritePin(GPIOA, 6, GPIO_PIN_RESET);//Yellow1
					GPIO_WritePin(GPIOB, 5, GPIO_PIN_RESET);//Red2
					GPIO_WritePin(GPIOB, 8, GPIO_PIN_RESET);//Traffic1
					ms_delay(1000);
			}
		}
}