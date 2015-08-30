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
* File Name    : r_camera.h
* Version      : V1.00
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for CMOS CAMERA module.
* Creation Date: 2015/6/25
***********************************************************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define WE   	P7.6
#define OE   	P7.3
#define RCK  	P0.6
#define WRST 	P7.2
#define RRST 	P7.1
#define CAMERA_DATA 	P2 
//#define H_RESOLUTION 80  //maximum:320,multiple of 4
//#define V_RESOLUTION 60  //maximum:240,multiple of 2
#define H_STEP 1    //for change Horizontal resolution
#define V_STEP 1    //for change Vertical resolution
#define THRESHOLD 0x96
#define ARRAY_SIZE 16

#define DISPLAY           //using windows software to display pictures grabbed by camera
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void User_Delay(int);
void R_FIFO_Create(void);
void R_FIFO_Write_Reset(void);
void R_FIFO_Read_Reset(void);
void R_Picture_Grab_Gray(void);
void R_Picture_Grab_Bin(void);
void R_Picture_Grab_Smoothing(void);
void R_OV7620_Configure(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
