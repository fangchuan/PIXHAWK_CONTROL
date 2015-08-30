/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial.c
* Version      : CodeGenerator for RL78/G13 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 2015/8/30
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "string.h"
#include "stdio.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_uart0_tx_address;        /* uart0 transmit buffer address */
volatile uint16_t  g_uart0_tx_count;           /* uart0 transmit data number */
volatile uint8_t * gp_uart0_rx_address;        /* uart0 receive buffer address */
volatile uint16_t  g_uart0_rx_count;           /* uart0 receive data number */
volatile uint16_t  g_uart0_rx_length;          /* uart0 receive data length */
volatile uint8_t   g_iic10_master_status_flag; /* iic10 start flag for send address check by master mode */
volatile uint8_t * gp_iic10_tx_address;        /* iic10 send data pointer by master mode */
volatile uint16_t  g_iic10_tx_count;           /* iic10 send data size by master mode */
volatile uint8_t * gp_iic10_rx_address;        /* iic10 receive data pointer by master mode */
volatile uint16_t  g_iic10_rx_count;           /* iic10 receive data size by master mode */
volatile uint16_t  g_iic10_rx_length;          /* iic10 receive data length by master mode */
/* Start user code for global. Do not edit comment generated here */
char buffer[50];
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0002_SAU_CK00_FCLK_2 | _0010_SAU_CK01_FCLK_1;
    R_UART0_Create();
    R_IIC10_Create();
}

/***********************************************************************************************************************
* Function Name: R_UART0_Create
* Description  : This function initializes the UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Create(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 receive and transmit */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;   /* disable INTSRE0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
    /* Set INTST0 low priority */
    STPR10 = 1U;
    STPR00 = 1U;
    /* Set INTSR0 low priority */
    SRPR10 = 1U;
    SRPR00 = 1U;
    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE |
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR00 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR00 = _8800_UART0_TRANSMIT_DIVISOR;
    NFEN0 |= _01_SAU_RXD0_FILTER_ON;
    SIR01 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR01 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL |
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR01 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR01 = _8800_UART0_RECEIVE_DIVISOR;
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;
    SOL0 |= _0000_SAU_CHANNEL0_NORMAL;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    /* Set RxD0 pin */
    PM1 |= 0x02U;
    /* Set TxD0 pin */
    P1 |= 0x04U;
    PM1 &= 0xFBU;
}

/***********************************************************************************************************************
* Function Name: R_UART0_Start
* Description  : This function starts the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Start(void)
{
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    STMK0 = 0U;    /* enable INTST0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SRMK0 = 0U;    /* enable INTSR0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
    SREMK0 = 0U;   /* enable INTSRE0 interrupt */
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
}

/***********************************************************************************************************************
* Function Name: R_UART0_Stop
* Description  : This function stops the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Stop(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 receive and transmit */
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART0 output */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;   /* disable INTSRE0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_UART0_Receive
* Description  : This function receives UART0 data.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART0_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_uart0_rx_count = 0U;
        g_uart0_rx_length = rx_num;
        gp_uart0_rx_address = rx_buf;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_UART0_Send
* Description  : This function sends UART0 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART0_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart0_tx_address = tx_buf;
        g_uart0_tx_count = tx_num;
        STMK0 = 1U;    /* disable INTST0 interrupt */
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
        STMK0 = 0U;    /* enable INTST0 interrupt */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_IIC10_Create
* Description  : This function initializes the IIC10 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC10_Create(void)
{
    ST0 |= _0004_SAU_CH2_STOP_TRG_ON;    /* disable IIC10 */
    IICMK10 = 1U;    /* disable INTIIC10 interrupt */
    IICIF10 = 0U;    /* clear INTIIC10 interrupt flag */
    /* Set INTIIC10 low priority */
    IICPR110 = 1U;
    IICPR010 = 1U;
    SIR02 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR02 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_EDGE_FALL | _0004_SAU_MODE_IIC | _0000_SAU_TRANSFER_END;
    SCR02 = _0000_SAU_TIMING_1 | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0000_SAU_MSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR02 = _9E00_IIC10_DIVISOR;
    SO0 |= _0400_SAU_CH2_CLOCK_OUTPUT_1 | _0004_SAU_CH2_DATA_OUTPUT_1;
    /* Set SCL10, SDA10 pin */
    PMC0 &= 0xF7U;
    P0 |= 0x18U;
    PM0 &= 0xE7U;
}

/***********************************************************************************************************************
* Function Name: R_IIC10_Master_Send
* Description  : This function starts transferring data for IIC10 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC10_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iic10_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear IIC10 master status flag */
    adr &= 0xFEU;    /* send mode */
    g_iic10_master_status_flag = _01_SAU_IIC_SEND_FLAG;            /* set master status flag */
    SCR02 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR02 |= _8000_SAU_TRANSMISSION;
    IICIF10 = 0U;   /* clear INTIIC10 interrupt flag */
    IICMK10 = 0U;   /* enable INTIIC10 */
    /* Set paramater */
    g_iic10_tx_count = tx_num;
    gp_iic10_tx_address = tx_buf;
    /* Start condition */
    R_IIC10_StartCondition();
    SIO10 = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC10_Master_Receive
* Description  : This function starts receiving data for IIC10 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC10_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iic10_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear master status flag */
    adr |= 0x01U;    /* receive mode */
    g_iic10_master_status_flag = _02_SAU_IIC_RECEIVE_FLAG;         /* set master status flag */
    SCR02 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR02 |= _8000_SAU_TRANSMISSION;
    IICIF10 = 0U;   /* clear INTIIC10 interrupt flag */
    IICMK10 = 0U;   /* enable INTIIC10 */
    /* Set parameter */
    g_iic10_rx_length = rx_num;
    g_iic10_rx_count = 0U;
    gp_iic10_rx_address = rx_buf;
    /* Start condition */
    R_IIC10_StartCondition();
    SIO10 = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC10_Stop
* Description  : This function stops the IIC10 operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC10_Stop(void)
{
    /* Stop transfer */
    ST0 |= _0004_SAU_CH2_STOP_TRG_ON;    /* disable IIC10 */
    IICMK10 = 1U;    /* disable INTIIC10 */
    IICIF10 = 0U;    /* clear INTIIC10 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_IIC10_StartCondition
* Description  : This function starts IIC10 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC10_StartCondition(void)
{
    volatile uint8_t w_count;
    
    SO0 &= ~_0004_SAU_CH2_DATA_OUTPUT_1;    /* clear IIC10 SDA */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= IIC10_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO0 &= ~_0400_SAU_CH2_CLOCK_OUTPUT_1;    /* clear IIC10 SCL */
    SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;            /* enable IIC10 output */
    SS0 |= _0004_SAU_CH2_START_TRG_ON;              /* enable IIC10 */
}

/***********************************************************************************************************************
* Function Name: R_IIC10_StopCondition
* Description  : This function stops IIC10 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC10_StopCondition(void)
{
    volatile uint8_t w_count;
    
    ST0 |= _0004_SAU_CH2_STOP_TRG_ON;           /* disable IIC10 */
    SOE0 &= ~_0004_SAU_CH2_OUTPUT_ENABLE;       /* disable IIC10 output */
    SO0 &= ~_0004_SAU_CH2_DATA_OUTPUT_1;        /* clear IIC10 SDA */
    SO0 |= _0400_SAU_CH2_CLOCK_OUTPUT_1; /* set IIC10 SCL */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= IIC10_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;         /* set IIC10 SDA */
}

/* Start user code for adding. Do not edit comment generated here */
MD_STATUS R_UART0_Send_Block(uint8_t * const tx_buf, uint16_t tx_num)
{
	int i, j;
	STMK0 = 1U;    /* disable INTST0 interrupt */
	for(i = 0; i < tx_num; i++)
	{
		TXD0 = *(tx_buf + i);
		while((SSR00 & 0x40));
	}
	return MD_OK;
}
extern unsigned char send_end;
void R_UART0_Send_Data(uint8_t * const tx_buf, uint16_t tx_num)
{
    R_UART0_Send(tx_buf,tx_num);
    while(!send_end);           //wait until data transfer end
    send_end = 0;
}

extern float current_height;
void return_hight(void)
{
     sprintf(buffer,"height:%d$\r\n",(int)(current_height*100));
     R_UART0_Send_Block(buffer,strlen(buffer));
}
extern float Roll,Pitch,Yaw;
void return_angle(void)
{
	 sprintf(buffer,"angle:%d$%d$%d\r\n",(int)Roll,(int)Pitch,(int)Yaw);
     R_UART0_Send_Block(buffer,strlen(buffer));
}
/* End user code. Do not edit comment generated here */
