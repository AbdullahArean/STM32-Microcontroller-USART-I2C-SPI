#include "CLOCK.h"
#include "GPIO.h"
#include "USART.h"
#include "SYS_INIT.h"
#include "TIMER.h"
#include "I2C.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define SLAVE_ADDRESS  0x44

uint32_t trafficTime=20,time_delay_cnt;//uint32_t
uint32_t TotalTimeCount=0;//uint32_t
int Flag = 0;
static char receive_buffer[50],output_buffer[100],input_buffer[100],str1[50],str2[50],ch,reply[100];
static int i=0,j=0,k=0,R1=10,R2=10,Y1=2,Y2=2,G1=10,G2=10,u1=5,u2=3;
static int FlagForSlave=0,Flag1_6=0;
static char Log[500][4][50];

void StringParsing(char *str);
void ControlToTraffic(char *s);
void ShowStatus(void);
void AllStatusPrint(void);
void AllReset(void);

void USART2_IRQHandler(void) {
	
	 if (USART2->SR & USART_SR_RXNE) 
	 {
			while(!(USART2->SR & USART_SR_RXNE));
			char ch=(uint8_t)USART2->DR;
			input_buffer[k++]= ch;
			USART2->SR &= ~USART_SR_RXNE;
		// UART_SendString(USART2,"Out");
			if(ch=='\0' || ch=='\n' || ch=='!')
			{
//				  input_buffer[k++]= '*';
//					input_buffer[k++]= '#';
				  input_buffer[k++]= '\0';
					Flag=1;
				  //UART_SendString(USART2,input_buffer);
					k=0;
			}
	 }
	 if (USART2->SR & USART_SR_TXE){
		 
				//USART2->DR =	output_buffer[k++];
        while(!(USART2->SR & USART_SR_TXE));
        USART2->SR &= ~(USART_SR_TXE);
        USART2->CR1 &= ~(USART_CR1_TXEIE);
    }
}

void StringParsing(char *str)
{
	  uint32_t number;
		//UART_SendString(USART2,str);
	  UART_SendString(USART2,"StringParse Successful\n");
	  sscanf(str,"%s %s %d",str1,str2,&number);
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
			return;
		}
}

void I2CMem_write(uint8_t slave_address,uint8_t *data)
{
	j=0;
	I2C_Start();
	slave_address = (uint8_t)(slave_address << 1);
	I2C_Address(slave_address);
	//UART_SendString(USART2,"Memwrite");
	I2C_WriteMulti(data);
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
	  sprintf(reply, "%d",TotalTimeCount);
		strcat(reply," traffic light 1");
		if(GPIO_ReadPin(GPIOA, 9)) //Red1
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOA, 8))//Green1
			strcat(reply," ON\n");
		else strcat(reply," OFF\n");
		strcpy(Log[TotalTimeCount][0],reply);
		//UART_SendString(USART2,reply);
		
		
		sprintf(reply, "%d",TotalTimeCount );
		strcat(reply," traffic light 2");
		if(GPIO_ReadPin(GPIOA, 12))//RED2
			strcat(reply," ON");
		else strcat(reply," OFF");
		if(GPIO_ReadPin(GPIOA, 11))//Green2
			strcat(reply," ON\n");
		else strcat(reply," OFF\n");
		strcpy(Log[TotalTimeCount][1],reply);
		//UART_SendString(USART2,reply);

		
		
		sprintf(reply, "%d",TotalTimeCount);
		strcat(reply," road north south");
		if(GPIO_ReadPin(GPIOC, 10))//North-South
			strcat(reply,"  heavy traffic\n");
		else strcat(reply,"  light traffic\n");
		strcpy(Log[TotalTimeCount][2],reply);
		//UART_SendString(USART2,reply);

		
		sprintf(reply, "%d",TotalTimeCount);
		strcat(reply," road east west");
		if(GPIO_ReadPin(GPIOC, 11))//East-West
			strcat(reply,"  heavy traffic\n");
		else strcat(reply,"  light traffic\n");
		strcpy(Log[TotalTimeCount][3],reply);
		//UART_SendString(USART2,reply);
		
		
		
}

void AllStatusPrint(void)
{
	int index;
	for(index = 2; index >=0 ;index--)
	{
		int temp=TotalTimeCount - (index*trafficTime);
		if(temp>=0){
			UART_SendString(USART2,Log[temp][0]);
			UART_SendString(USART2,Log[temp][1]);
			UART_SendString(USART2,Log[temp][2]);
			UART_SendString(USART2,Log[temp][3]);
		}
		else break;
	}
	
	return;

}

void ControlToTraffic(char *s)
{
	I2CMem_write(SLAVE_ADDRESS,s);
}



void I2C2_EV_IRQHandler(void)
{
		//UART_SendString(USART2,"sobar bahire!");
    if (I2C2->SR1 & I2C_SR1_ADDR) {
				
        // Address matched, clear the ADDR flag
        (void)I2C2->SR2;
    }

    if (I2C2->SR1 & I2C_SR1_RXNE) {
        // Data received, read the data register
				uint8_t tmp;
        tmp = I2C2->DR;
				output_buffer[j++]=tmp;
				//char send[50];
				//sprintf(send,"%c",tmp);
			  //UART_SendString(USART2,send);
				if(tmp=='!')
				{
						UART_SendString(USART2,"Recevied Done!\n");
						output_buffer[j++]='\0';
						if(strlen(output_buffer) > 8)
						{
							UART_SendString(USART2,output_buffer);
						  StringParsing(output_buffer);
						}
						//FlagForSlave=1;
						strcpy(output_buffer,"");
					  j=0;
				}
    }
	//   /* if (I2C2->SR1 & I2C_SR1_TXE) {
//        // Data transmitted, write the next byte to the DR register
//        I2C2->DR = i2c_data[1];
//    }*/
}

void AllReset(void)
{
	GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);//Green1
	GPIO_WritePin(GPIOB, 5, GPIO_PIN_RESET);//Green1
	GPIO_WritePin(GPIOB, 1, GPIO_PIN_RESET);//Traffic1
	GPIO_WritePin(GPIOB, 2, GPIO_PIN_RESET);//Traffic2
	GPIO_WritePin(GPIOB, 4, GPIO_PIN_RESET);//Red2
	GPIO_WritePin(GPIOB, 6, GPIO_PIN_RESET);//Red1
}
	
	
	int main(void)
	{
		//Initialization
		initClock();
		sysInit();
		USART2_Config();
		GPIOx_MODER_Set();
		TIM6Config();
		strcpy(input_buffer,"");
		strcpy(output_buffer,"");
		int MorS=0;
		int outflag=1;
		if(MorS==1)
			I2C1_Config();
		else	
			I2C2_Config();
		
	  AllReset();
		while(1)
	  {
			//sprintf(send,"%d\n",flag);
			//UART_SendString(USART2,"Mehadi");
			if(MorS==1){
				if(outflag==1)
				{
					UART_SendString(USART2,"From Master");
					outflag=0;
				}
						if(Flag == 1)
						{ 
								//UART_SendString(USART2,"Hasan");
								ControlToTraffic(input_buffer);
								Flag=0;
								strcpy(input_buffer,"");
						}
					}
			else{
						if(outflag==1)
							{
								UART_SendString(USART2,"From Slave");
								outflag=0;
							}
						int RandomNum = rand()%2+1;
						if(RandomNum==1)
						{
								GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);//Green1
								GPIO_WritePin(GPIOC, 9, GPIO_PIN_SET);//Red2
								GPIO_WritePin(GPIOB, 1, GPIO_PIN_SET);//Traffic1
								time_delay_cnt= (G1 + u2)*1000;
								ms_delay((uint32_t)time_delay_cnt);
								time_delay_cnt = Y1*1000;
								ms_delay((uint32_t)time_delay_cnt);
								GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);//Green2
								GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);//Red1
							  GPIO_WritePin(GPIOB, 2, GPIO_PIN_SET);//Traffic2
								GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);//Green1
								GPIO_WritePin(GPIOC, 9, GPIO_PIN_RESET);//Red2
								GPIO_WritePin(GPIOB, 1, GPIO_PIN_RESET);//Traffic1
								time_delay_cnt= (G2 + u1)*1000;
								ms_delay((uint32_t)time_delay_cnt);
								time_delay_cnt = Y2*1000;
								ms_delay((uint32_t)time_delay_cnt);
							  AllReset();
								ms_delay(1000);
						}
						else
						{
								GPIO_WritePin(GPIOB, 6, GPIO_PIN_SET);//Red1
							  GPIO_WritePin(GPIOB, 2, GPIO_PIN_SET);//Traffic2
								GPIO_WritePin(GPIOB, 7, GPIO_PIN_SET);//Green2
								time_delay_cnt= (G2 + u1)*1000;
								ms_delay((uint32_t)time_delay_cnt);
								time_delay_cnt = Y2*1000;
								ms_delay((uint32_t)time_delay_cnt);
								GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);//Green1
							  GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);//Green2
								GPIO_WritePin(GPIOB, 1, GPIO_PIN_SET);//Traffic1
								GPIO_WritePin(GPIOB, 2, GPIO_PIN_RESET);//Traffic2
								GPIO_WritePin(GPIOC, 9, GPIO_PIN_SET);//Red2
							  GPIO_WritePin(GPIOB, 6, GPIO_PIN_RESET);//Red1
								time_delay_cnt= (G1 + u2)*1000;
								ms_delay((uint32_t)time_delay_cnt);
								time_delay_cnt = Y1*1000;
								ms_delay((uint32_t)time_delay_cnt);
							  AllReset();
							  ms_delay(1000);
						}
						
//						if(FlagForSlave==1)
//						{
//							FlagForSlave=0;
							//slave side data rcv in array out_buffer, parse and config
							//UART_SendString(USART2,output_buffer);
							

						//}
					}
		}
	}