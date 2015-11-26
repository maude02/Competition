//Laboratoire 7/8
//Maude Carrier 1684023
//Henri Labonté 1688692
#ifndef __FONCTION__H__
#define __FONCTION__H__
#include "tm4c123gh6pm.h" //Pour avoir acces au GPIO registers de PORTF
#include <stdint.h>
#include "Delay.h"
#include "stdint.h"
#define __FPU_PRESENT 1
#define ARM_MATH_CM4
#include "arm_math.h"


void initGPIO (void);

void DisableInterrupts(void);

void EnableInterrupts(void);

void initSysTick(void);

void initUART(void);

void initADC0(void);

void UART_OutChar(char data);

uint8_t statusBouton (void);

void initTimerZero(void);
	
#endif
