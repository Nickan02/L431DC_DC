/*
 * @Author       : Leeer
 * @Date         : 2022-07-11 18:08:38
 * @FilePath     : \MDK-ARMd:\keil\CubePreserve\L431DC_DC\Drivers\USART\My_Usart.C
 * @Description  :
 */
#include "My_Usart.H"

unsigned char UART1_Rx_Buf[MAX_REC_LENGTH] = {0}; // USART1�洢��������
unsigned char UART1_Rx_flg = 0;                   // USART1������ɱ�־
unsigned int UART1_Rx_cnt = 0;                    // USART1�������ݼ�����
unsigned char UART1_temp[REC_LENGTH] = {0};       // USART1�������ݻ���

/**
 * @Description  : ʹ��DMAʱ���ܷ��͹��죬Ŀ��
 *                  ���ֻ��2500us������ܱ�֤��������
 *                ��ѯģʽû����������
 * @param         {char} *format
 * @return        {*}
 */
void Usart1_Printf(char *format, ...)
{
  // VA_LIST ����C�����н����������һ��꣬����ͷ�ļ���#include <stdarg.h>�����ڻ�ȡ��ȷ�������Ĳ�����
  static uint16_t UartTxBuf[200];
  uint16_t len = 0;
  va_list args;
  va_start(args, format);
  len = vsnprintf((char *)UartTxBuf, sizeof(UartTxBuf), (char *)format, args);
  va_end(args);
#if USART_DMA_TX_ENABLE
  HAL_UART_Transmit_DMA(&huart1, &UartTxBuf, len);
#else
  HAL_UART_Transmit(&huart1, (uint8_t *)&UartTxBuf, len, 10);
#endif
}
void Usart1_Scanf(char *Rx_Buf)
{
  if (UART1_Rx_flg)
  {
    strcpy(Rx_Buf, UART1_Rx_Buf);
    memset(UART1_Rx_Buf, 0, sizeof(UART1_Rx_Buf));
    UART1_Rx_cnt = 0;
    UART1_Rx_flg = 0;
  }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    UART1_Rx_Buf[UART1_Rx_cnt] = UART1_temp[0];
    UART1_Rx_cnt++;
    //  if(0x0a == UART1_temp[0])   //�ж��Ƿ�����/r/n
    UART1_Rx_flg = 1;
#if USART_DMA_RX_ENABLE
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_temp, 1);
#else
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&UART1_temp, 1);
#endif
  }
}