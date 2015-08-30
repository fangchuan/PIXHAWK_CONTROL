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
* File Name    : r_camera.c
* Version      : V1.00
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 2015/6/25
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
#include "r_camera.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
unsigned long write_fifo_count = 0;      //write count
unsigned char picture[ARRAY_SIZE]={0};  //data of picture store in this array 
uint16_t H_RESOLUTION = 80;
uint16_t V_RESOLUTION = 60;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_FIFO_Create
* Description  : This function initialize the fifo.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_FIFO_Create(void)
{
	WE = 0U;              	//disable write
	OE = 1U;		//disable read
	R_FIFO_Write_Reset();
	R_FIFO_Read_Reset();
}

/***********************************************************************************************************************
* Function Name: R_FIFO_Write_Reset
* Description  : The data of FIFO input address will be set to 0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_FIFO_Write_Reset(void)
{
	WRST = 0U;        //enable write reset
	User_Delay(1);
	WRST = 1U;   
}

/***********************************************************************************************************************
* Function Name: R_FIFO_Read_Reset
* Description  : The data of FIFO output address will be set to 0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_FIFO_Read_Reset(void)
{
	RRST = 0U;	  //enable read reset
	User_Delay(1);
	RCK = 1U;
	User_Delay(1);
	RCK = 0U;
	User_Delay(1);
	RRST = 1U;    
}

/***********************************************************************************************************************
* Function Name: User_Delay
* Description  : CPU will wait for a period of time according to i;
* Arguments    : i -
*                    delay time
* Return Value : None
***********************************************************************************************************************/
void User_Delay(int i)
{
	int j,k;
	for(j=i;j--;j>0)
		for(k=100;k--;k>0);
}
/***********************************************************************************************************************
* Function Name: R_Picture_Grab_Gray
* Description  : This function grab gray data of pictures from camera.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Picture_Grab_Gray(void)
{
	unsigned char pixel=0,bin_data=0,count=0;
	unsigned char i_rem,j_rem;
	unsigned int i=0,j=0,k=0;
	unsigned long read_fifo_count=0;
	
	OE = 0U;					//enable FIFO ouput
    R_FIFO_Read_Reset(); 				//set the read address to 0
	
    for(i=1;i<=V_RESOLUTION;i++)			//read each lines
	{	
	    for(j=1;j<=H_RESOLUTION+2;j++)		//read each pixels
		{			
		    if(write_fifo_count>read_fifo_count) //if write_fifo_count>read_fifo_count you can read the fifo
		    {
        		read_fifo_count++;
			    
		        RCK = 1U;         		//read clock set high

		    if(j<=H_RESOLUTION)		//remove the blanking signal added by hardware
			{
			    i_rem=i%H_STEP;
			    j_rem=j%V_STEP;
			    if((i_rem==0)&(j_rem==0))   //change resolution by software
			    {
			        pixel = CAMERA_DATA;    //read the value of D[7:0]
				#ifdef DISPLAY
				  R_UART0_Send_Data(&pixel,1);
				#endif
			     }
			}
			
			RCK = 0U; 
		   }
		     else{j--;}
		}
	}
	OE = 1U;
	write_fifo_count = 0;
	read_fifo_count = 0;
}

/***********************************************************************************************************************
* Function Name: R_Picture_Grab_Bin
* Description  : This function grab binary data of pictures from camera.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Picture_Grab_Bin(void)
{
	unsigned char pixel=0,bin_data=0,count=0;
	unsigned char i_rem,j_rem;
	unsigned int i=0,j=0,k=0;
	unsigned long read_fifo_count=0;
	
	OE = 0U;					//enable FIFO ouput
    R_FIFO_Read_Reset(); 				//set the read address to 0
	
    for(i=1;i<=V_RESOLUTION;i++)			//read each lines
	{	
	    for(j=1;j<=H_RESOLUTION+2;j++)		//read each pixels
		{
		    if(write_fifo_count>read_fifo_count) //if write_fifo_count>read_fifo_count you can read the fifo
		    {
        		read_fifo_count++;
			
        		RCK = 1U;         		//read clock set high

		    if(j<=H_RESOLUTION)		//remove the blanking signal added by hardware
			{
		        i_rem=i%H_STEP;
			    j_rem=j%V_STEP;
			    if((i_rem==0)&(j_rem==0))     //change resolution by software
			    {  
			          pixel = CAMERA_DATA;    //read the value of D[7:0]
					
			    if(pixel >= THRESHOLD)
				         bin_data|=0x01;
				else  
				         bin_data&=0xfe; 
				     count++;
				if(count > 7)
				  {
				    #ifdef DISPLAY
				      R_UART0_Send_Data(&bin_data,1);//binary data to host to use URAT0
				    #endif		  
				    count = 0;
				    if(k<ARRAY_SIZE)
				      picture[k++]=bin_data; //binary data to memory
				  }
				 else
				 {
				    bin_data=bin_data<<1;					  
				  }
			      }
			}
			
			RCK = 0U; 
		     }
		     else{j--;}
		}
	}
	OE = 1U;
	write_fifo_count = 0;
	read_fifo_count = 0;
}

/***********************************************************************************************************************
* Function Name: R_Picture_Grab_Smoothing
* Description  : This function grab binary data of pictures from camera.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Picture_Grab_Smoothing(void)
{
	unsigned char pixel=0,bin_data=0,count=0,last_num=0x00,current_num=0x00,next_num=0x00;
	unsigned char i_rem,j_rem;
	unsigned int i=0,j=0,k=0;
	
	OE = 0U;					//enable FIFO ouput
    R_FIFO_Read_Reset(); 				//set the read address to 0
	
    for(i=1;i<=V_RESOLUTION;i++)			//read each lines
	{	
	    for(j=1;j<=H_RESOLUTION+2;j++)		//read each pixels
		{			
        		RCK = 1U;         		//read clock set high

		if(j<=H_RESOLUTION+1)		//remove the blanking signal added by hardware
		{
		      i_rem=i%H_STEP;
			  j_rem=j%V_STEP;
		   if((i_rem==0)&(j_rem==0))     //change resolution by software
		   {
				pixel = CAMERA_DATA;    //read the value of D[7:0]
				
				if(j<=H_RESOLUTION)
				{
				   if(pixel >= THRESHOLD)
					     next_num = 0x01;
				   else
					     next_num = 0x00;
				}
				else
				{
					  next_num = 0x01;
				}
			    if(j-1)
				{
					  switch(current_num)
					  {
						  case 0x00:
						    if(last_num&next_num)
						       bin_data|=0x01;
						    else
						       bin_data&=0xfe;break;
						  case 0x01:
						    if(last_num|next_num)
						       bin_data|=0x01;
						    else
						       bin_data&=0xfe;break;
					  }
					
					  count++;
					  if(count > 7)
					  {
					    #ifdef DISPLAY
					      R_UART0_Send_Data(&bin_data,1);//binary data to host to use URAT0
					    #endif		  
					    count = 0;
					    if(k<ARRAY_SIZE)
					      picture[k++]=bin_data; //binary data to memory
					  }
					  else
					  {
					    bin_data=bin_data<<1;					  
					  }
					  last_num = current_num;
				  }
				  else
				  {
					  last_num = 0x01;
				  }  
				  current_num = next_num;
			      }
			}
			
			RCK = 0U; 
		}
	}
	OE = 1U;
}

/***********************************************************************************************************************
* Function Name: R_OV7620_Configure
* Description  : The function is config the register of OV7620.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_OV7620_Configure(void)
{
    unsigned char command[2] = {0x00,0x00};
    unsigned char h_dec = (320-H_RESOLUTION)/4;
    unsigned char v_dec = (240-V_RESOLUTION)/2;
    
    command[0] = 0x14;
    command[1] = 0x24;
    R_IIC10_Master_Send(0x42, command, 2U);  // change to QVGA mode
    User_Delay(1000);
    
    command[0] = 0x17;
    command[1] = 47+h_dec;
    R_IIC10_Master_Send(0x42, command, 2U);  // set Horizontal Window start
    User_Delay(1000);
    
    command[0] = 0x18;
    command[1] = 208-h_dec;
    R_IIC10_Master_Send(0x42, command, 2U);  // set Horizontal Window end
    User_Delay(1000);
    
    command[0] = 0x19;
    command[1] = 6+v_dec;
    R_IIC10_Master_Send(0x42, command, 2U);  // set Vertical Window start
    User_Delay(1000);
    
    command[0] = 0x1A;
    command[1] = 245-v_dec;
    R_IIC10_Master_Send(0x42, command, 2U);  // set Vertical Window end
    User_Delay(1000);
    
    
}