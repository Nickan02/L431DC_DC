
#ifndef __MYUSART_H__
#define __MYUSART_H__

#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"

#define REC_LENGTH  1
#define MAX_REC_LENGTH  1024 
extern unsigned char UART1_Rx_Buf[MAX_REC_LENGTH];
extern unsigned char UART1_Rx_flg ;
extern unsigned int  UART1_Rx_cnt ;
extern unsigned char UART1_temp[1];
extern UART_HandleTypeDef huart1;

#define USART_DMA_TX_ENABLE	0  //是否使能发送DMA 1是 0否
#define USART_DMA_RX_ENABLE	0  //是否使能接收DMA 1是 0否
void Usart1_Printf(char *format, ...);
void Usart1_Scanf(char *Rx_Buf);

#endif /* __MYUSART_H__ */
