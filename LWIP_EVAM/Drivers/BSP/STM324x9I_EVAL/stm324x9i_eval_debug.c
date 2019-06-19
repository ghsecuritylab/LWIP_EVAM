/**********************************************************************************
            Copyright (C), 2015-2025, Jingyi Tech. Co., Ltd.
            
  文件名: usart_debug.c
  作  者: wubing
  版  本: v1.0.0         
  日  期: 2015-05-27
  描  述: 

  历史修改记录:
  1----------------------------------------------------                
    <维护人>                  <日期>           <版本>                  
    Grotesque.Nrw.Brg        2017.9.22          V1.0                   
    <描述>                                                                                  
*********************************************************************************/

#include "stm324x9i_eval_debug.h"	

#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <stdarg.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup STM324x9I_EVAL_SD_Private_Variables SD Private Variables
  * @{
  */
/* UART handler declaration */
UART_HandleTypeDef UartHandle;


__weak static void Error_Handler(void)
{

  while(1)
  {
  }
}
/**
  * @brief  Initializes the UART mode according to the specified parameters in
  *         the UART_InitTypeDef and create the associated handle.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
void BSP_USART_Debug_Init()
{
    
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART1 configured as follow:
    - Word Length = 8 Bits
    - Stop Bit = One Stop bit
    - Parity = ODD parity
    - BaudRate = 115200 baud
    - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance = USARTx;

    UartHandle.Init.BaudRate = 256000;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}



/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
	
      GPIO_InitTypeDef  GPIO_InitStruct;
      
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      USARTx_TX_GPIO_CLK_ENABLE();
      USARTx_RX_GPIO_CLK_ENABLE();
      /* Enable USART1 clock */
      USARTx_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = USARTx_TX_PIN;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = USARTx_TX_AF;
      
      HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
        
      /* UART RX GPIO pin configuration  */
      GPIO_InitStruct.Pin = USARTx_RX_PIN;
      GPIO_InitStruct.Alternate = USARTx_RX_AF;
        
      HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

}
/********************************************************************
* 功 能: 串口发送函数
*
* 参 数:  参考 : printf()    
*
* 返 回:    VOID             
*
* 创 建: 吴兵   2015-05-22
*
* 维 护: 戴杰   2016-01-06
*
* 其他 ：1) 该函数最大支持字符流为256字节，
*******************************************************************************/
void Printf(const char * fmt, ...)
{
	/*
    //OS_ERR err;    
    char  buf_str[256];
    char  *p_str;
    va_list   v_args;
	
    //OSMutexPend(&BSP_SerLock_1, 10, OS_OPT_PEND_BLOCKING, (CPU_TS*)0, &err);
	
    //if (err != OS_ERR_NONE)return ;

	memset(buf_str,0,sizeof(buf_str));
	
    va_start(v_args, fmt);
   (void)vsnprintf((char          *)&buf_str[0],
                   (unsigned  int  ) sizeof(buf_str),
                   (char const    *) fmt,
                                     v_args);
    va_end(v_args);

    p_str = buf_str;    
    while((*p_str) != (char )0)
    {
        while ((USART2->SR & USART_FLAG_TC) == RESET);
        USART2->DR = (*p_str & (uint16_t)0x01FF);
        p_str++;
    }
	while ((USART2->SR & USART_FLAG_TC) == RESET); //等待发送结束  


	//OSMutexPost(&BSP_SerLock_1,OS_OPT_NONE,  &err);
	*/
}


#include "lcd_log_conf.h"
#if 0
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}


/************************ (C) COPYRIGHT Grotesque.Nrw.Brg *****END OF FILE****/