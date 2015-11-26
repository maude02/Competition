//Compétition
//Maude Carrier 1684023
//Henri Labonte 1688692
//David Charbonneau 1689816
//Marc-Antoine Gareau 
#include "SSD2119.h"
#include "fonction.h"

#include "tm4c123gh6pm.h" //Pour avoir acces au GPIO registers de PORTF
#include <stdint.h>
#include <stdio.h>
#include "Delay.h"
#include "stdint.h"

#define __FPU_PRESENT 1
#define ARM_MATH_CM4
#include "arm_math.h"
#include "arm_const_structs.h"


int main(void)
{
	DisableInterrupts(); // Permet de ne pas avoir d'interruption durant l'initialisation
	LCD_Init();
	initGPIO();	//Appel des fonctions d'initialisation 
	initSysTick();
	initUART();
	initADC0();	
	EnableInterrupts(); // Active les interruptions pour la suite de l'exécution du programme

	__asm
	{
		WFI
	}

	while(1){} // Boucle vide
}

void GPIOF_Handler(void)
{
	
}

void ADC0SS3_Handler(void)
{
	
}
