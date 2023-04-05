#include "stm32f446xx.h"
#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "GlobalVar.h"
#include <stdio.h>
#include<string.h>
#include<stdlib.h>


void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART6_IRQHandler(void);
void stringParse(char *s);
void TIM6Config(void);
void GPIOPin_MODER_Set(void);
void TIM6_DAC_IRQHandler(void);
void USRAT6to1(void);
void USART1to6(void);

void USART6to1(void)
{
		i=0;
		j=0;
	  flagfor6to1=1,flagfor1to6=0;
		ms_delay(1000);
		while(i<=strlen(input_buffer))
		{
				USART6->CR1 |= USART_CR1_TXEIE; 
				ms_delay(100);
				i++;j++;
		}
		//recevie[i]='\0';
		//ms_delay(1000);
		//UART_SendString(USART2,recevie);
}
void USART1to6(void)
{
		i=0;
		j=0;
	  flagfor6to1=0,flagfor1to6=1;
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
void display(void)
{
	  UART_SendString(USART2,"MEHAdi-Arean\n");
	  sprintf(reply, "%d",totaltime );
		strcat(reply," traffic light 1");
		if(GPIO_ReadPin(GPIOC, 0)) //Red
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOC, 1))// Yellow
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOB, 0))//Green
			strcat(reply," ON\n");
		else strcat(reply," OFF\n");
		//UART_SendString(USART2,reply);
		USART1to6();
		// For Others 3
		sprintf(reply, "%d",totaltime );
		strcat(reply," traffic light 2");
		if(GPIO_ReadPin(GPIOC, 13))
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOC, 14))
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOC, 15))
			strcat(reply," ON\n");
		else strcat(reply," OFF\n");
		//UART_SendString(USART2,reply);
		USART1to6();
		
		sprintf(reply, "%d",totaltime );
		strcat(reply," road north south");
		if(GPIO_ReadPin(GPIOC, 10))
			strcat(reply,"  heavy traffic\n");
		else strcat(reply,"  light traffic\n");
		//UART_SendString(USART2,reply);
		USART1to6();
		
		sprintf(reply, "%d",totaltime );
		strcat(reply," road east west");
		if(GPIO_ReadPin(GPIOC, 12))
			strcat(reply,"  heavy traffic\n");
		else strcat(reply,"  light traffic\n");
		 //UART_SendString(USART2,reply);
		 USART1to6();
}
void stringParse(char *s)
{
	  sscanf(s,"%s %s %d",s1,s2,&traffic);
    //printf("%s %s %d\n",s1,s2,traffic);
		UART_SendString(USART2,s);
	  UART_SendString(USART2," YES Perse Successful\n");
    if(strcmp(s2,"light")==0)
    {
				//UART_SendString(USART2,s);
        if(traffic==1)
        {
            sscanf(s,"%s %s %d %c %c %c %d %d %d %d",s1,s2,&traffic,&ch,&ch,&ch,&G1,&Y1,&R1,&T1);
            //printf("\n%d %d %d %d",G1,Y1,R1,T1);
        }
        else
        {
            sscanf(s,"%s %s %d %c %c %c %d %d %d %d",s1,s2,&traffic,&ch,&ch,&ch,&G2,&Y2,&R2,&T2);
            //printf("\n%d %d %d %d",G2,Y2,R2,T2);
        }
    }
}
void USART1_IRQHandler(void){
		if((USART1->SR & USART_SR_RXNE) && flagfor6to1==1 ){
		while(!(USART1->SR & USART_SR_RXNE));
		recevie[i] = (uint8_t)USART1->DR;
		USART1->SR &= ~(USART_SR_RXNE);
		if(recevie[i]=='\0' || recevie[i]=='\n')
		{
			  //UART_SendString(USART2,recevie);
			  stringParse(recevie);
		}
  }
	if((USART1->SR & USART_SR_TXE ) && flagfor1to6==1){
		USART1->DR = reply[i];
		while(!(USART1->SR & USART_SR_TXE));
		USART1->SR &= ~(USART_SR_TXE);
		USART1->CR1 &= ~(USART_CR1_TXEIE);
	}
}
void USART6_IRQHandler(void){
		 if((USART6->SR & USART_SR_RXNE) && flagfor1to6==1){
		 while(!(USART6->SR & USART_SR_RXNE));
		 output_buffer[j] = (uint8_t)USART6->DR;
		 USART6->SR &= ~(USART_SR_RXNE);
			 if(output_buffer[j]=='\n' || output_buffer[j]=='\0')
			 {
					//UART_SendString(USART2,output_buffer);

			 }
		}
		
		if((USART6->SR & USART_SR_TXE) && flagfor6to1==1 ){
		USART6->DR = input_buffer[j];
		while(!(USART6->SR & USART_SR_TXE));
		USART6->SR &= ~(USART_SR_TXE);
		USART6->CR1 &= ~(USART_CR1_TXEIE);
	}
}

void USART2_IRQHandler(void) {
	
	 if (USART2->SR & USART_SR_RXNE) 
	 {
			while(!(USART2->SR & USART_SR_RXNE));
			char ch=(uint8_t)USART2->DR;
			input_buffer[in++]= ch;
			USART2->SR &= ~USART_SR_RXNE;
			if(ch=='\0' || ch=='\n')
			{
					input_buffer[in]= '\0';
					flag=1;
					in=0;
			}
	 }
	 if (USART2->SR & USART_SR_TXE){
        //USART2->DR =	con_buffer_outp[con_out];
        while(!(USART2->SR & USART_SR_TXE));
        USART2->SR &= ~(USART_SR_TXE);
        USART2->CR1 &= ~(USART_CR1_TXEIE);
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
		GPIOPin_MODER_Set();
		TIM6Config();
	  while(1)
	  {
			if(flag == 1)
			{ 
					USART6to1();
				  //flag=0;
//				  ms_delay(1000);
//				  if(flag==1)
//						USART6to1();
//					flag=0;
//					ms_delay(1000);
//					if(flag==1)
//						USART6to1();

			}
			GPIO_WritePin(GPIOA, 7, GPIO_PIN_SET);//Green1
			GPIO_WritePin(GPIOB, 8, GPIO_PIN_SET);//Traffic1
			GPIO_WritePin(GPIOB, 9, GPIO_PIN_SET);//Traffic2
			GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);//Red2
			time_delay= (G1 + T2)*1000;
			ms_delay(time_delay);
			GPIO_WritePin(GPIOA, 7, GPIO_PIN_RESET);//Green1
			GPIO_WritePin(GPIOA, 6, GPIO_PIN_SET);//Yellow1
			display();
			time_delay = Y1*1000;
			ms_delay(time_delay);
			GPIO_WritePin(GPIOA, 6, GPIO_PIN_RESET);///Yellow1
			GPIO_WritePin(GPIOA, 5, GPIO_PIN_SET);//Red1
			GPIO_WritePin(GPIOB, 9, GPIO_PIN_RESET);//Traffic2
			GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);//Green2
			GPIO_WritePin(GPIOB, 5, GPIO_PIN_RESET);//Red2
			time_delay= (G2 + T1)*1000;
						display();

			ms_delay(time_delay);
			GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);//Green2
			GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);//Yellow2
			time_delay = Y2*1000;
			ms_delay(time_delay);
			GPIO_WritePin(GPIOB, 6, GPIO_PIN_RESET);//Yellow2
			GPIO_WritePin(GPIOA, 5, GPIO_PIN_RESET);//Red1
			GPIO_WritePin(GPIOB, 8, GPIO_PIN_RESET);//Traffic1
			ms_delay(1000);
			sprintf(reply,"%d",totaltime);
			//UART_SendString(USART2,reply);
			/*
			GPIO_WritePin(GPIOA, 6, GPIO_PIN_SET);
			GPIO_WritePin(GPIOA, 7, GPIO_PIN_SET);
			ms_delay(3000);
			GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);
			GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);
			GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);
			ms_delay(3000);
			GPIO_WritePin(GPIOA, 5, GPIO_PIN_RESET);
			GPIO_WritePin(GPIOA, 6, GPIO_PIN_RESET);
			GPIO_WritePin(GPIOA, 7, GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, 5, GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, 6, GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);
			ms_delay(3000);
			GPIO_WritePin(GPIOB, 8, GPIO_PIN_SET);
			GPIO_WritePin(GPIOB, 9, GPIO_PIN_SET);
			ms_delay(3000);
			GPIO_WritePin(GPIOB, 8, GPIO_PIN_RESET);
			GPIO_WritePin(GPIOB, 9, GPIO_PIN_RESET);
			ms_delay(3000);
			*/
				//display();
				//totaltime++;
				//sprintf(reply, "%d",totaltime );
		    //UART_SendString(USART2,reply);

	  }
		/*
		strcpy(send,"Dipto");
		strcpy(recevie,"Shaha ddddd");
		UART_SendString(USART2,recevie);
	  while(1)
		{
			  strcpy(recevie,"");
			  i=0;
			  j=0;
				ms_delay(1000);
			  while(i<strlen(send))
				{
						USART6->CR1 |= USART_CR1_TXEIE; 
						ms_delay(100);
						i++;j++;
				}
				recevie[i]='\0';
				//ms_delay(1000);
				UART_SendString(USART2,recevie);
				ms_delay(1000);
		}
		*/
		
		/*while(1)
		{
			 if(R1>0) 
			 {
				   GPIO_WritePin(GPIOA, 5, GPIO_PIN_SET);
			 }
				 

		}*/
		
}