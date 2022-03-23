 /************************************************************************/
/*																		*/
/*	main.c	--	Main program module for project							*/
/*																		*/
/************************************************************************/
/*	Author: 	Dion Moses												*/
/*	Copyright 2009, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This program is a reference design for the Digilent	Basic			*/
/*	Robotic Development Kit (RDK-Basic) with the Cerebot 32MX4 			*/
/*	Microcontroller board.  It uses two timers to drive two motors 		*/
/*	with output compare modules.										*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	 12/09/09(DionM): created											*/
/*   12/29/09(LeviB): altered to add movement functions and PmodBtn and */
/*					  PmodSwt functionality								*/
/*	 12/08/10(AaronO): renamed to RDK_Basic								*/	
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <plib.h>
#include "stdtypes.h"
#include "config.h"
#include "MtrCtrl.h"
#include "spi.h"
#include "util.h"
#include "PID.h"
#include <math.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

#define		TCKPS22 			6
#define 	TCKPS21				5
#define 	TCKPS20				4

#define		TCKPS32 			6
#define 	TCKPS31				5
#define 	TCKPS30				4


#define     ON_OFF                      1  
#define     DIST_BUF_SIZE               5
#define     SPEED_SET_L_FORWARD         0.7f
#define     SPEED_SET_R_FORWARD         0.7f
#define     SPEED_SET_L_LEFT            0.7f
#define     SPEED_SET_R_LEFT            0.75f
#define     SPEED_SET_L_RIGHT           0.75f
#define     SPEED_SET_R_RIGHT           0.7f
/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
#ifndef OVERRIDE_CONFIG_BITS

#pragma config ICESEL   = ICS_PGx2		// ICE/ICD Comm Channel Select
#pragma config BWP      = OFF			// Boot Flash Write Protect
#pragma config CP       = OFF			// Code Protect
#pragma config FNOSC    = PRIPLL		// Oscillator Selection
#pragma config FSOSCEN  = OFF			// Secondary Oscillator Enable
#pragma config IESO     = OFF			// Internal/External Switch-over
#pragma config POSCMOD  = HS			// Primary Oscillator
#pragma config OSCIOFNC = OFF			// CLKO Enable
#pragma config FPBDIV   = DIV_8			// Peripheral Clock divisor
#pragma config FCKSM    = CSDCMD		// Clock Switching & Fail Safe Clock Monitor
#pragma config WDTPS    = PS1			// Watchdog Timer Postscale
#pragma config FWDTEN   = OFF			// Watchdog Timer 
#pragma config FPLLIDIV = DIV_2			// PLL Input Divider
#pragma config FPLLMUL  = MUL_16		// PLL Multiplier
#pragma config UPLLIDIV = DIV_2			// USB PLL Input Divider
#pragma config UPLLEN   = OFF			// USB PLL Enabled
#pragma config FPLLODIV = DIV_1			// PLL Output Divider
#pragma config PWP      = OFF			// Program Flash Write Protect
#pragma config DEBUG    = OFF			// Debugger Enable/Disable
    
#endif

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

#define	stPressed	1
#define	stReleased	0

#define	cstMaxCnt	10 // number of consecutive reads required for
					   // the state of a button to be updated

struct btn {
	BYTE	stBtn;	// status of the button (pressed or released)
	BYTE	stCur;  // current read state of the button
	BYTE	stPrev; // previous read state of the button
	BYTE	cst;	// number of consecutive reads of the same button 
					// state
};

//PmodCLS instructions
static	char szClearScreen[] = { 0x1B, '[', 'j', 0};

static	char szCursorOff[] = { 0x1B, '[', '0', 'c', 0 };
static	char szBacklightOn[]     = { 0x1B, '[', '3', 'e', 0 };

static	char szScrollLeft[] = {0x1B, '[', '1', '@', 0}; 
static	char szScrollRight[] = {0x1B, '[', '1', 'A', 0}; 
static	char szWrapMode[] = {0x1B, '[', '0', 'h', 0}; 

static	char szCursorPos[] = {0x1B, '[', '1', ';', '0', 'H', 0}; 
/* ------------------------------------------------------------ */
/*				Global Variables				                */
/* ------------------------------------------------------------ */

volatile	struct btn	btnBtn1;
volatile	struct btn	btnBtn2;

volatile	struct btn	PmodBtn1;
volatile	struct btn	PmodBtn2;
volatile	struct btn	PmodBtn3;
volatile	struct btn	PmodBtn4;

volatile	struct btn	PmodSwt1;
volatile	struct btn	PmodSwt2;
volatile	struct btn	PmodSwt3;
volatile	struct btn	PmodSwt4;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void	DeviceInit(void);
void	AppInit(void);
void	Wait_ms(WORD ms);
void    button_debounce(void);
void ADC_init(void);

// PID Globals
uint16_t IC2_data[6];
uint16_t IC3_data[6];
uint16_t IC2_data_index = 0;
uint16_t IC3_data_index = 0;
uint32_t IC2_counter = 0;
uint32_t IC3_counter = 0;
volatile float IC2_avg = 0.0;
volatile float IC3_avg = 0.0;
volatile float IC2_speed_SP = 0.0;
volatile float IC3_speed_SP = 0.0;

// ADC Globals
volatile float ADCValue0 = 0.0;
volatile float ADCValue1 = 0.0;
volatile float ADCValue2 = 0.0;
volatile float distance_1 = 0.0;
volatile float distance_2 = 0.0;
uint32_t ADC_Count = 0;

void __ISR(_INPUT_CAPTURE_2_VECTOR, ipl5) IC2_IntHandler(void) {
    IFS0 &= ~(1 << 9);   // Clears interrupt flag
    
    // Turn LED On
    prtLed2Set = (1<<bnLed2);
    
    // Read data until empty
    uint16_t data = 0;
    
    while((IC2CON & 0x00000008) == 0x00000008) {
            data = (IC2BUF & 0x0000FFFF);
            
            int i = 0;
            // Save data to global array
            for(i = 0; i < 5; i++) {
                IC2_data[i] = IC2_data[i+1];
            }
            IC2_data[5] = data;
            
            IC2_counter++;
    }
    
    // Average time 
    int32_t sum = 0;
    int32_t temp = 0;
    int i = 0;
    for(i = 5; i > 0; i--) {
        temp = IC2_data[i] - IC2_data[i-1];
        
        if(temp < 0) {
            sum = sum + temp + 65000;
        }
        else {
            sum = sum + temp;
        }
    }
    IC2_avg = sum / 5.0;
   
    // Turn LED off
    prtLed2Clr = (1 << bnLed2);
}

void __ISR(_INPUT_CAPTURE_3_VECTOR, ipl5) IC3_IntHandler(void) {
    IFS0 &= ~(1 << 13);   // Clears interrupt flag
    
    // Turn LED On
   // prtLed3Set = (1<<bnLed3);
    
    // Read data until empty
    uint16_t data = 0;
    
    while((IC3CON & 0x00000008) == 0x00000008) {
            data = (IC3BUF & 0x0000FFFF);
            
            int i = 0;
            // Save data to global array
            for(i = 0; i < 5; i++) {
                IC3_data[i] = IC3_data[i+1];
            }
            IC3_data[5] = data;
            
            IC3_counter++;
    }
    
    // Average time 
    int32_t sum = 0;
    int32_t temp = 0;
    int i = 0;
    for(i = 5; i > 0; i--) {
        temp = IC3_data[i] - IC3_data[i-1];
        
        if(temp < 0) {
            sum = sum + temp + 65000;
        }
        else {
            sum = sum + temp;
        }
    }
    IC3_avg = sum / 5.0;
    
    
    // Turn LED off
   // prtLed3Clr = (1 << bnLed3);
}

/* ------------------------------------------------------------ */
/*				Interrupt Service Routines						*/
/* ------------------------------------------------------------ */
/***	Timer5Handler
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Interrupt service routine for Timer 5 interrupt. Timer 5
**		is used to perform software debouncing of the on-board
**		buttons. It is also used as a time base for updating
**		the on-board LEDs and the Pmod8LD LEDs at a regular interval.
*/


void __ISR(_TIMER_5_VECTOR, ipl7) Timer5Handler(void)
{
    // Turn LED On
    prtLed1Set = (1<<bnLed1);
	mT5ClearIntFlag();
    
//    char buffer[20];
//    uint8_t char_num; 
    
	
    // PID Loop Stuff
    if (IC2_counter > 100) {
        float IC2_avg_time = IC2_avg * 0.000001;    // time in seconds
        float IC2_speed = 0.0045/IC2_avg_time;      // 0.0045ft/pulse

        float error = PID_error_2(IC2_speed_SP, IC2_speed);
        PID_update_2(error);
        IC2_counter = 0;
        
//        char_num = sprintf(buffer, "Speed: %f", IC2_speed);
//        SpiPutBuff(szClearScreen, 3);
//        SpiPutBuff(buffer, char_num);
	}
    
    // PID Loop Stuff
    if (IC3_counter > 100) {
        float IC3_avg_time = IC3_avg * 0.000001;    // time in seconds
        float IC3_speed = 0.0045/IC3_avg_time;      // 0.0045ft/pulse

        float error = PID_error_3(IC3_speed_SP, IC3_speed);
        PID_update_3(error);
        IC3_counter = 0;
	}
    
    button_debounce();
    
    // Turn LED off
    prtLed1Clr = (1 << bnLed1);
}

void __ISR(_ADC_VECTOR, ipl3) _ADC_IntHandler(void) 
{

//   ADC CONVERSION
//   ISR is written assuming that ADC channels are scanned and the interrupt is thrown after all the conversions are complete
//
    float distance_1_buf[DIST_BUF_SIZE];
    float distance_2_buf[DIST_BUF_SIZE];
    
    char buffer[20];
    uint8_t char_num; 
    float distance_diff = 0.0;

	prtLed3Set = (1 << bnLed3);   		// turn LED3 on in the beginning of interrupt
	IFS1CLR = ( 1 << 1 );  			// clear interrupt flag for ADC1 Convert Done

//  Read the a/d buffers and convert to voltages
	//ADCValue0 = (float)ADC1BUF0*3.3f/1023.0f;	// Reading AN0(zero), pin 1 of connector JJ -- servo sensor (center)
    ADCValue1 = (float)ADC1BUF1*3.3/1023.0;
    distance_1_buf[ADC_Count] = 25.409f * powf(ADCValue1,-1.327f) + 2.0;
    
    
    ADCValue2 = (float)ADC1BUF2*3.3/1023.0;
    distance_2_buf[ADC_Count] = 25.409f * powf(ADCValue2,-1.327f);
    
    if (ADC_Count   == (DIST_BUF_SIZE - 1)) {
        
        float distance_1_sum = 0.0;
        float distance_2_sum = 0.0;
        int i = 0;
        
        
        for (i = 0; i < DIST_BUF_SIZE; i++) {
            distance_1_sum += distance_1_buf[i];
            distance_2_sum += distance_2_buf[i];
        }
        
        distance_1 = distance_1_sum / DIST_BUF_SIZE;
        distance_2 = distance_2_sum / DIST_BUF_SIZE;
        
        distance_diff = distance_1 - distance_2;
        
        char_num = sprintf(buffer, "%.1f     %.1f", distance_1, distance_2);
        SpiPutBuff(szClearScreen, 3);
        SpiPutBuff(buffer, char_num);
        
        if (distance_diff > 1.5f)  {
            IC2_speed_SP = SPEED_SET_L_LEFT;
            IC3_speed_SP = SPEED_SET_R_LEFT;
        }else if (distance_diff < -1.5f) {
            IC2_speed_SP = SPEED_SET_L_RIGHT;
            IC3_speed_SP = SPEED_SET_R_RIGHT;
        }
        else {
            IC2_speed_SP = SPEED_SET_L_FORWARD;
            IC3_speed_SP = SPEED_SET_R_FORWARD;
        }
        ADC_Count = 0;
    }
	
    ADC_Count++;
	
	prtLed3Clr = (1 << bnLed3);   // turn LED3 off at the end of interrupt
}


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	main
**
**	Synopsis:
**		st = main()
**
**	Parameters:
**		none
**
**	Return Values:
**		does not return
**
**	Errors:
**		none
**
**	Description:
**		Main program module. Performs basic board initialization
**		and then enters the main program loop.
*/

int main(void) {

	BYTE	stBtn1;
	BYTE	stBtn2;

	BYTE	stPmodBtn1;
	BYTE	stPmodBtn2;
	BYTE	stPmodBtn3;
	BYTE	stPmodBtn4;

	BYTE	stPmodSwt1;
	BYTE	stPmodSwt2;
	BYTE	stPmodSwt3;
	BYTE	stPmodSwt4;


	DeviceInit();
	AppInit();

	INTDisableInterrupts();
	DelayMs(500);
	

	//write to PmodCLS
	SpiEnable();
	SpiPutBuff(szClearScreen, 3);
	DelayMs(4);
	SpiPutBuff(szBacklightOn, 4);
	DelayMs(4);
	SpiPutBuff(szCursorOff, 4);
	DelayMs(4);
	SpiPutBuff("Hello from", 10);
	DelayMs(4);
	SpiPutBuff(szCursorPos, 6);
	DelayMs(4);
	SpiPutBuff("Digilent!", 9);
	DelayMs(2000);
	//SpiDisable();
    
    // Wheel Test
    if(ON_OFF) {
        OC2R = 2500;
        OC2RS = 2500;
        OC3R = 2500;
        OC3RS = 2500;
    }
    
     IC2_speed_SP = SPEED_SET_L_FORWARD;
     IC3_speed_SP = SPEED_SET_R_FORWARD;
    
	prtLed1Set	= ( 1 << bnLed1 );
	INTEnableInterrupts();
	while (fTrue)
	{		
		INTDisableInterrupts();
      
		//get data here
		stBtn1 = btnBtn1.stBtn;
		stBtn2 = btnBtn2.stBtn;

		stPmodBtn1 = PmodBtn1.stBtn;
		stPmodBtn2 = PmodBtn2.stBtn;
		stPmodBtn3 = PmodBtn3.stBtn;
		stPmodBtn4 = PmodBtn4.stBtn;

		stPmodSwt1 = PmodSwt1.stBtn;
		stPmodSwt2 = PmodSwt2.stBtn;
		stPmodSwt3 = PmodSwt3.stBtn;
		stPmodSwt4 = PmodSwt4.stBtn;

		INTEnableInterrupts();
        
        
       
        
        
		//configure OCR to go forward
        
//        if (IC2_counter <= 1565) {
//            OC2R = 9999;
//            OC2RS = 9999;
//        } else {
//            OC2R = 0;
//            OC2RS = 0;
//        }
//        
//        if (IC3_counter <= 1565) {
//            OC3R = 9999;
//            OC3RS = 9999;
//        } else {
//            OC3R = 0;
//            OC3RS = 0;
//        }
        
        /*
        // Wheel Test
        OC2R = 2500;
        OC2RS = 2500;
        OC3R = 2500;
        OC3RS = 2500;
        
        Wait_ms(500);
        
        OC2R = 0;
        OC2RS = 0;
        OC3R = 0;
        OC3RS = 0;
        
        Wait_ms(500);
        */
        
        /*
		if(stPressed == stPmodBtn1){
			//start motor if button 2 pressed

			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00);
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			
		}else if(stPressed == stPmodBtn2){
			//start left turn

			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00);
			MtrCtrlBwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			
		}else if(stPressed == stPmodBtn3){
			//start right turn

			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00);
			MtrCtrlRight();
			UpdateMotors();
			Wait_ms(0x0200);
			MtrCtrlStop();
			UpdateMotors();

		} else if(stPressed == stPmodBtn4){
			//start move backward

			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00);
			MtrCtrlLeft();
			UpdateMotors();
			Wait_ms(0x0200);
			MtrCtrlStop();
			UpdateMotors();

		} else if(stPressed == stPmodSwt1){
			//make square to right

			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00);  //gives delay to toggle switch
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlRight();
			UpdateMotors();		// first turn
			Wait_ms(0x0180);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlRight();     // second turn
			UpdateMotors();
			Wait_ms(0x0180);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlRight();		// third turn
			UpdateMotors();
			Wait_ms(0x0180);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlRight();
			UpdateMotors();
			Wait_ms(0x0180);
			MtrCtrlStop();
			UpdateMotors();

		} else if(stPressed == stPmodSwt2){
			//make triangle to left

			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00); //gives delay to toggle switch
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlLeft();  	//first turn
			UpdateMotors();
			Wait_ms(0x0280);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlLeft();		//second turn
			UpdateMotors();
			Wait_ms(0x0280);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlLeft();		//third turn
			UpdateMotors();
			Wait_ms(0x0280);
			MtrCtrlStop();
			UpdateMotors();
		
		}else if(stPressed == stPmodSwt3){
			// Three point turn around

			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00);  //gives delay to toggle switch
			MtrCtrlFwdRight();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlBwdLeft();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0500);
			MtrCtrlFwd();
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();

		}else if(stPressed == stPmodSwt4){
			// dance
			
			MtrCtrlStop();  //stop before sending new data to avoid possible short circuit
			UpdateMotors(); 
			Wait_ms(0x0A00);  //gives delay to toggle switch
			MtrCtrlFwdLeft(); // step left
			UpdateMotors();
			Wait_ms(0x0300);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0200);
			MtrCtrlFwdRight(); // step right
			UpdateMotors();		
			Wait_ms(0x0300);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0200);
			MtrCtrlFwdLeft();  // step left
			UpdateMotors();
			Wait_ms(0x0300);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0200);
			MtrCtrlFwdRight(); // step right
			UpdateMotors();
			Wait_ms(0x0200);
			MtrCtrlStop();
			UpdateMotors();
			Wait_ms(0x0200);
			MtrCtrlLeft();     // spin
			UpdateMotors();
			Wait_ms(0x0800);
			MtrCtrlStop();
			UpdateMotors();
		}  //end if
         */
	}  //end while
}  //end main

/* ------------------------------------------------------------ */
/***	DeviceInit
**
**	Synopsis:
**		DeviceInit()
**
**	Parameters:
**		none
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initializes on chip peripheral devices to the default
**		state.
*/

void DeviceInit() {

	// Configure left motor direction pin and set default direction.
	trisMtrLeftDirClr	= ( 1 << bnMtrLeftDir );
	prtMtrLeftDirSet	= ( 1 << bnMtrLeftDir );	// forward
	
	// Configure right motor direction pin and set default direction.
	trisMtrRightDirClr	= ( 1 << bnMtrRightDir );	
	prtMtrRightDirClr	= ( 1 << bnMtrRightDir );	// forward
    
    // PID Initialization 
    PID_init_2(250.0, 2500.0, 0.0, -100000.0, 100000.0);
    PID_init_3(250.0, 2500.0, 0.0, -100000.0, 100000.0);

	// Configure Output Compare 2 to drive the left motor.
	OC2CON	= (1 << 3)|( 1 << 2 ) | ( 1 << 1 );	// pwm set up and timer3
	OC2R	= dtcMtrStopped;
	OC2RS	= dtcMtrStopped;

	// Configure Output Compare 3.
	OC3CON = ( 1 << 3 ) | ( 1 << 2 ) | ( 1 << 1 );	// pwm set up and timer3
	OC3R	= dtcMtrStopped;
	OC3RS	= dtcMtrStopped;

	// Configure Timer 2.
	TMR2	= 0;									// clear timer 2 count
	PR2		= 64999;

	// Configure Timer 3.
	TMR3	= 0;
	PR3		= 9999;

	// Start timers and output compare units.
	T2CON		= ( 1 << 15 ) | ( 1 << TCKPS20 )|(1 << TCKPS21);		// timer 2 prescale = 8
	OC2CONSET	= ( 1 << 15 );	// enable output compare module 2
	OC3CONSET	= ( 1 << 15 );	// enable output compare module 3
	T3CON		= ( 1 << 15 ) | ( 1 << TCKPS31 ) | ( 1 << TCKPS30); 	// timer 3 prescale = 8

	// Configure Timer 5.
	TMR5	= 0;
	PR5		= 499; // period match every 100 us
	IPC5SET	= ( 1 << 4 ) | ( 1 << 3 ) | ( 1 << 2 ) | ( 1 << 1 ) | ( 1 << 0 ); // interrupt priority level 7, sub 3
	IFS0CLR = ( 1 << 20);
	IEC0SET	= ( 1 << 20);
	
	// Start timers.
	T5CON = ( 1 << 15 ) | ( 1 << 5 ) | ( 1 << 4 ); // fTimer5 = fPb / 8
    
	//enable SPI
	SpiInit();
    
    //enable LED
    trisLed1Clr = (1 << bnLed1)|(1 << bnLed2)|(1 << bnLed3);
    prtLed1Clr = (1 << bnLed1)|(1 << bnLed2)|(1 << bnLed3);
    
    // Configure IC2
    IPC2SET = (1 << 12) | (1 << 10)| (1 << 9) | (1 << 8);    // Set priority 5, sub 3
    IPC2CLR = (1 << 11);                // Set priority 5
  
    IC2CON &= ~(1 << 8);    // 16b timer
    IC2CON |= (1 << 7);     // Timer2
    IC2CON &= ~((1 << 6)|(1 << 5)); // Interrupt on every capture event
    IC2CON |= (1 << 1)|(1 << 0);
    IC2CON &= ~(1 << 2);
    IC2CON |= (1 << 15);    // Enables Module
    
    IEC0 |= (1 << 9);   // Enable interrupt 
    
    // Configure IC3
    IPC3SET = (1 << 12) | (1 << 10)| (1 << 9);    // Set priority 5, sub 2
    IPC3CLR = (1 << 11) | (1 << 8);                // Set priority 5
  
    IC3CON &= ~(1 << 8);    // 16b timer
    IC3CON |= (1 << 7);     // Timer2
    IC3CON &= ~((1 << 6)|(1 << 5)); // Interrupt on every capture event
    IC3CON |= (1 << 1)|(1 << 0);
    IC3CON &= ~(1 << 2);
    IC3CON |= (1 << 15);    // Enables Module
    
    IEC0 |= (1 << 13);   // Enable interrupt 
    
    ADC_init();
    

	// Enable multi-vector interrupts.
	INTEnableSystemMultiVectoredInt();
}

/* ------------------------------------------------------------ */
/***	AppInit
**
**	Synopsis:
**		AppInit()
**
**	Parameters:
**		none
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Performs application specific initialization. Sets devices
**		and global variables for application.
*/

void AppInit() {



}


/* ------------------------------------------------------------ */
/***	Wait_ms
**
**	Synopsis:
**		Wait_ms(WORD)
**
**	Parameters:
**		WORD (range from 0 to 65535)
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Will wait for specified number of milliseconds.  Using a 
**		word variable allows for delays up to 65.535 seconds.  The value
**		in the for statement may need to be altered depending on how your
**		compiler translates this code into AVR assembly.  In assembly, it is
**		possible to generate exact delay values by counting the number of clock
**		cycles your loop takes to execute.  When writing code in C, the compiler
**		interprets the code, and translates it into assembly.  This process is 
**		notoriously inefficient and may vary between different versions of AVR Studio
**		and WinAVR GCC.  A handy method of calibrating the delay loop is to write a 
**		short program that toggles an LED on and off once per second using this 
**		function and using a watch to time how long it is actually taking to
**		complete. 
**
*/

void Wait_ms(WORD delay) {

	WORD i;

	while(delay > 0){

		for( i = 0; i < 375; i ++){
			;;
		}
		delay -= 1;
	}
}

void button_debounce(void){
    
    static	WORD tusLeds = 0;
    
    // Read the raw state of the button pins.
	btnBtn1.stCur = ( prtBtn1 & ( 1 << bnBtn1 ) ) ? stPressed : stReleased;
	btnBtn2.stCur = ( prtBtn2 & ( 1 << bnBtn2 ) ) ? stPressed : stReleased;
	
	//Read the raw state of the PmodBTN pins
	PmodBtn1.stCur = ( prtJE1 & ( 1 << bnJE1 ) ) ? stPressed : stReleased;
	PmodBtn2.stCur = ( prtJE2 & ( 1 << bnJE2 ) ) ? stPressed : stReleased;
	PmodBtn3.stCur = ( prtJE3 & ( 1 << bnJE3 ) ) ? stPressed : stReleased;
	PmodBtn4.stCur = ( prtJE4 & ( 1 << bnJE4 ) ) ? stPressed : stReleased;

	//Read the raw state of the PmodSWT pins
	PmodSwt1.stCur = ( prtJA1 & ( 1 << swtJA1 ) ) ? stPressed : stReleased;
	PmodSwt2.stCur = ( prtJA2 & ( 1 << swtJA2 ) ) ? stPressed : stReleased;
	PmodSwt3.stCur = ( prtJA3 & ( 1 << swtJA3 ) ) ? stPressed : stReleased;
	PmodSwt4.stCur = ( prtJA4 & ( 1 << swtJA4 ) ) ? stPressed : stReleased;

	// Update state counts.
	btnBtn1.cst = ( btnBtn1.stCur == btnBtn1.stPrev ) ? btnBtn1.cst + 1 : 0;
	btnBtn2.cst = ( btnBtn2.stCur == btnBtn2.stPrev ) ? btnBtn2.cst + 1 : 0;

	//Update state counts for PmodBTN
	PmodBtn1.cst = (PmodBtn1.stCur == PmodBtn1.stPrev) ? PmodBtn1.cst +1 : 0;
	PmodBtn2.cst = (PmodBtn2.stCur == PmodBtn2.stPrev) ? PmodBtn2.cst +1 : 0;
	PmodBtn3.cst = (PmodBtn3.stCur == PmodBtn3.stPrev) ? PmodBtn3.cst +1 : 0;
	PmodBtn4.cst = (PmodBtn4.stCur == PmodBtn4.stPrev) ? PmodBtn4.cst +1 : 0;

	//Update state counts for PmodSWT
	PmodSwt1.cst = (PmodSwt1.stCur == PmodSwt1.stPrev) ? PmodSwt1.cst +1 : 0;
	PmodSwt2.cst = (PmodSwt2.stCur == PmodSwt2.stPrev) ? PmodSwt2.cst +1 : 0;
	PmodSwt3.cst = (PmodSwt3.stCur == PmodSwt3.stPrev) ? PmodSwt3.cst +1 : 0;
	PmodSwt4.cst = (PmodSwt4.stCur == PmodSwt4.stPrev) ? PmodSwt4.cst +1 : 0;
	
	// Save the current state.
	btnBtn1.stPrev = btnBtn1.stCur;
	btnBtn2.stPrev = btnBtn2.stCur;

	// Save the current state for PmodBTN
	PmodBtn1.stPrev = PmodBtn1.stCur;
	PmodBtn2.stPrev = PmodBtn2.stCur;
	PmodBtn3.stPrev = PmodBtn3.stCur;
	PmodBtn4.stPrev = PmodBtn4.stCur;

	// Save the current state for PmodSWT
	PmodSwt1.stPrev = PmodSwt1.stCur;
	PmodSwt2.stPrev = PmodSwt2.stCur;
	PmodSwt3.stPrev = PmodSwt3.stCur;
	PmodSwt4.stPrev = PmodSwt4.stCur;
	
	// Update the state of button 1 if necessary.
	if ( cstMaxCnt == btnBtn1.cst ) {
		btnBtn1.stBtn = btnBtn1.stCur;
		btnBtn1.cst = 0;
	}
	
	// Update the state of button 2 if necessary.
	if ( cstMaxCnt == btnBtn2.cst ) {
		btnBtn2.stBtn = btnBtn2.stCur;
		btnBtn2.cst = 0;
	}

	//if statements for buttons

	// Update the state of PmodBTN1 if necessary.
	if ( cstMaxCnt == PmodBtn1.cst ) {
		PmodBtn1.stBtn = PmodBtn1.stCur;
		PmodBtn1.cst = 0;
	}
	
	// Update the state of PmodBTN2 if necessary.
	if ( cstMaxCnt == PmodBtn2.cst ) {
		PmodBtn2.stBtn = PmodBtn2.stCur;
		PmodBtn2.cst = 0;
	}

	// Update the state of PmodBTN3 if necessary.
	if ( cstMaxCnt == PmodBtn3.cst ) {
		PmodBtn3.stBtn = PmodBtn3.stCur;
		PmodBtn3.cst = 0;
	}

	// Update the state of PmodBTN4 if necessary.
	if ( cstMaxCnt == PmodBtn4.cst ) {
		PmodBtn4.stBtn = PmodBtn4.stCur;
		PmodBtn4.cst = 0;
	}

	//if statements for switches

	// Update the state of PmodSWT1 if necessary.
	if ( cstMaxCnt == PmodSwt1.cst ) {
		PmodSwt1.stBtn = PmodSwt1.stCur;
		PmodSwt1.cst = 0;
	}
	
	// Update the state of PmodSWT2 if necessary.
	if ( cstMaxCnt == PmodSwt2.cst ) {
		PmodSwt2.stBtn = PmodSwt2.stCur;
		PmodSwt2.cst = 0;
	}

	// Update the state of PmodSWT3 if necessary.
	if ( cstMaxCnt == PmodSwt3.cst ) {
		PmodSwt3.stBtn = PmodSwt3.stCur;
		PmodSwt3.cst = 0;
	}

	// Update the state of PmodSWT4 if necessary.
	if ( cstMaxCnt == PmodSwt4.cst ) {
		PmodSwt4.stBtn = PmodSwt4.stCur;
		PmodSwt4.cst = 0;
	}
}

void ADC_init(void) {
    AD1PCFG	= 0XFFF8;
	// CONFIGURE AN0, AN1, and AN2 AS ANALOG INPUTS
	AD1CON1	= 0X00E4;   
	// BIT 7-5 SSRC 111 = INTERNAL COUNTER ENDS SAMPLING AND STARTS CONVERSION
	// BIT 4 CLRASM 0 = Normal Operation, buffer contents will be overwritten by the next conversion sequence
	// BIT 2 ASAM 1 = SAMPLING BEGINS immediately after conversion completes; SAMP bit is automatically set
	// BIT 1 SAMP 0 = ADC IS NOT SAMPLING
	// BUT 0 DONE 0 = STATUS BIT
	AD1CON2	= 0X0408; // 0000 0100 0000 1000     				
	// BIT 10 CSCNA 1 = SCAN INPUTS
	// BIT 2-3 SMPL 1-1 = ONE INTERRUPT AFTER EVERY THIRD CONVERSION
	AD1CON3	= 0X1FFF;
	// BIT 15 ADRC 0 = CLOCK DERIVED FROM PERIPHERAL BUS CLOCK
	// SAMC AND ADCS - I NEED TO READ MORE ABOUT TIMING TO UNDERSTAND THE FUNCTION OF THESE TWO VARIABLES
	AD1CHS	= 0X00000000;   // 32 bit SFR  
	AD1CSSL	= 0X0007;  
	// CSSL = SCAN CHANNELS 2,1 and 0
	IPC6SET	= ( 1 << 27 ) | ( 1 << 26 ); // ADC interrupt priority level 3, sub 0
	IFS1CLR	= 2;    // CLEAR ADC INTERRUPT FLAG
	IEC1SET = 2;	// ENABLE ADC INTERRUPT
	AD1CON1SET = 0X8000;	// 	TURN ADC ON
}
/************************************************************************/
