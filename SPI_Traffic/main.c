#include "CLOCK.h"
#include "GPIO.h"
#include "USART.h"
#include "SYS_INIT.h"
#include "TIMER.h"
#include "SPI.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "stm32f4xx.h"

//#define SLAVE_ADDRESS  0x44

uint32_t trafficTime=20,time_delay_cnt;//uint32_t
uint32_t TotalTimeCount=0;//uint32_t
int Flag = 0,FlagSPI=0;
static char output_buffer[100],receive_buffer[100],input_buffer[100],str1[50],str2[50],ch,reply[100];
static int i=0,j=0,k=0,R1=10,R2=10,Y1=2,Y2=2,G1=10,G2=10,u1=5,u2=3;
static int FlagForSlave=0,Flag1_6=0;
static char Log[500][4][50];

void StringParsing(char *str);
void ControlToTraffic(char *s,int size);
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
				  UART_SendString(USART2,input_buffer);
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
			//ShowStatus();
			return;
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

void ControlToTraffic(char *s,int size)
{
	SPI_Transmit(s,size);
}
void SPI_Transmit (uint8_t *data, int size)
{
	
	/************** STEPS TO FOLLOW *****************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	************************************************/		
	UART_SendString(USART2,"\nInTransmission!\n");

	int i=0;
	while (i<size)
	{
	   while (!((SPI1->SR) & (1<<1)));  // wait for TXE bit to set -> This will indicate that the buffer is empty
	   while(SPI1->SR & (1<<7));
		 SPI1->DR = data[i];  // load the data into the Data Register
		 ms_delay(40);
	   i++;
	}	
	
	
/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPI1->SR)&(1<<1)));  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI1->SR)&(1<<7)));  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	
	//  Clear the Overrun flag by reading DR and SR
	uint8_t clr;
	clr = (uint8_t) SPI1->DR;
	clr = (uint8_t) SPI1->SR;
	UART_SendString(USART2,"SendDone!\n");
	
}

void SPI_DataReception (void)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/		
  char ch = 0;
	int i = 0;
		UART_SendString(USART2,"\nIn Receive!");

	while(ch != '!'){
		while(!((SPI1->SR) & (1<<0)));
		ch = (uint8_t)SPI1->DR;
		if(ch != '!'){
			char temp[50];
			sprintf(temp,"%c",ch);
			UART_SendString(USART2,temp);
			output_buffer[i++] = ch;
		}
		else break;
	}
	
	output_buffer[i] = '\0';
	UART_SendString(USART2,"\nReceiveDone!");

}

void SPI1_IRQHandler(void){
	
	SPI1->CR2 &= ~SPI_CR2_RXNEIE;
	
	if(SPI1->SR & SPI_SR_RXNE)
	{
		  UART_SendString(USART2,"\nFromHandeler!");

			SPI_DataReception();
			FlagSPI = 1;
			//UART_SendString(USART2,output_buffer);
			StringParsing(output_buffer);
	}
	
	
//	if(strlen(rcv_str) != 0){
//		sendString("RCV ::: ");
//		sendString(rcv_str);
//		sendString(" :::\n");
//		strcpy(input_buff,rcv_str);
//		parseCommand(input_buff);
//	}
	
	SPI1->CR2 |= SPI_CR2_RXNEIE;
}


void AllReset(void)
{
	GPIO_WritePin(GPIOB, 7, GPIO_PIN_RESET);//Green1
	GPIO_WritePin(GPIOB, 5, GPIO_PIN_RESET);//Green1
	GPIO_WritePin(GPIOB, 1, GPIO_PIN_RESET);//Traffic1
	GPIO_WritePin(GPIOB, 2, GPIO_PIN_RESET);//Traffic2
	GPIO_WritePin(GPIOC, 9, GPIO_PIN_RESET);//Red2
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
		int MorS=1;
		int outflag=1;
		if(MorS==1)
			SPIConfigMaster();
		else	
		{
			SPIConfigSlave();
			AllReset();
		}
		
	  
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
						ControlToTraffic(input_buffer,strlen(input_buffer));
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
						int RandomNum = 1;//rand()%2+1;
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