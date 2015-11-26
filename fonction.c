//Laboratoire 7/8
//Maude Carrier 1684023
//Henri Labonté 1688692
#include "fonction.h"

void initGPIO (void)	// Pour ajuster les LED et les Switch (inspiree de la page 154 du manuel)
{
	SYSCTL_RCGCGPIO_R |= 0x00000020;	// Active la clock pour le PORTF
	while((SYSCTL_PRGPIO_R&0x00000020) == 0)	{};	// Assure que la clock est prete a utilisation
	GPIO_PORTF_LOCK_R = 0x4C4F434B;		//Deverouille le PORTF
	GPIO_PORTF_CR_R = 0x1F;		// Active les modifications/utilisation des ports 0,1,2,3 et 4 du PORTF
	GPIO_PORTF_DIR_R = 0x0E;	// Identification des ports d'entrees (ports 0 et 4 initialises a la valeur 0) et de sortie (ports 1,2 et 3 initialises a la valeur 1)
	GPIO_PORTF_PUR_R = 0x11;	// Active le "pull-up" pour les ports 0 et 4
	GPIO_PORTF_DEN_R = 0x1F;	// Pour commencer a utiliser les ports 0 et 4 
	//GPIO_PORTF_IS_R =	0xFF;				// ça met une interruption pour le front montant seulement
	GPIO_PORTF_IBE_R = 0x11; 	// permet une interruption sur les deux edges
	GPIO_PORTF_RIS_R = 0x00;	// Set en mode "aucune interruption"
	GPIO_PORTF_IM_R = 0x11;		// Active les interruptions pour les bits 4 et 0 (deux SW)
	NVIC_EN0_R = 0x40000000;	//Enable
	NVIC_PRI7_R = 0x00400000;	//Priorité de l'interruption
}

void DisableInterrupts(void)
{
 __asm // Desactivation des interuptions sur le uControleur
	{
		CPSID I
	}
}

void EnableInterrupts(void)
{
 __asm // Activation des interuptions sur le uControleur
	{
		CPSIE I
	}
}

void initSysTick(void)
{
	SYSCTL_RCC2_R |= 0x80000000;	//Active le bit USERCC2 dans le registre (31e bit)
	SYSCTL_RCC2_R |= 0x00000800;	//Active le BYPASS2 (PLL bypass). Permet de valider les infos qu'on va changer avant d'activer la clock
	SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) + 0x00000540; //Libere les bit 10 à 6 et set la valeur (0x15) qui correspond à 16MHz
	SYSCTL_RCC2_R &= ~0x00000070;	//Configuration de l'oscillateur source (fait par défaut)
	SYSCTL_RCC2_R &= ~0x00002000;	//Desactive PWRDN (fait par défaut)
	SYSCTL_RCC2_R |= 0x40000000; 	//Met le 400MHz PLL
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R &~ 0x1FC00000) + 0x01000000;	//On set les bits 28 à 22 à 4 (pour 80MHz)
	while((SYSCTL_RIS_R & 0x00000040) == 0) {};
	SYSCTL_RCC2_R &= ~0x00000800;	//Desactive le bypass
	NVIC_ST_CTRL_R = 0x0000000;		//Initialisation du sysTick
	NVIC_ST_RELOAD_R = 16000000-1;		//Pour initialiser a 1ms - 80000-1 / 200ms 16000000-1
	NVIC_ST_CURRENT_R = 0;				//Initialiser à 0
	NVIC_ST_CTRL_R = 0x5;					//active le systick
}

void initUART(void)
{
	SYSCTL_RCGCUART_R |= 0x01;	// Activate UART0 ( Inspirer de la page 321 du manuel de Valvano)
	SYSCTL_RCGCGPIO_R |= 0x01;	// Activation de port A
	UART0_CTL_R &= ~0x01;				// Disable UART (Universal Asynchronous Receivers/Transmitters)
	UART0_IBRD_R = 43;					// 80MHz/(16*115200)= int(43.4027778)
	UART0_FBRD_R = 26;					// 0.40*64=25.777779 = 26
	UART0_LCRH_R = 0x0070;			// 8-bits words length, enable FIFO
	UART0_CTL_R = 0x0301;				// Enable RXE, TXE and UART
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;	//UART
	GPIO_PORTA_AMSEL_R &= ~0x03;//Disable analog function on PAL-0
	GPIO_PORTA_AFSEL_R |= 0x03;	//Enable alt function on PAL-0
	GPIO_PORTA_DEN_R |= 0x03;		// Enable digital I/O on PAL-0
}

void initADC0(void)
{
	SYSCTL_RCGCADC_R |= 0x01;	//Active ADC0
	SYSCTL_RCGCGPIO_R |= 0x00000010;	// Active la clock pour le PORTE
	while((SYSCTL_PRGPIO_R&0x00000010) == 0)	{};	// Assure que la clock est prete a utilisation
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x00000008;
	GPIO_PORTE_DIR_R &=~ 0x08;	// Identification des ports d'entrees
	GPIO_PORTE_AFSEL_R |= 0X08;	//Pour activer  le port 3 
	GPIO_PORTE_DEN_R &=~ 0x08;	// Pour désactiver le port 3 
	GPIO_PORTE_AMSEL_R |= 0x08;	//Active les fonctions analogues du port 3
	
	SYSCTL_RCGCADC_R |= 0x00000001;	// Active la clock pour le ADC
	while((SYSCTL_RCGCADC_R&0x00000001) == 0)	{};
		
	ADC0_PC_R = 0x01;	//Configure pour 125kHz
	ADC0_SSPRI_R = 0x0123;	//Met la priorité au maximum
	ADC0_ACTSS_R &=~ 0x0008;	//Désactiver le sample sequencer 3
	//ADC0_EMUX_R &=~ 0xF000;
	ADC0_EMUX_R |= 0x05000;	//Trigger de la séquence 3
	initTimerZero();
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0);	//Efface le champ SS3
	ADC0_SSCTL3_R = 0x00000006;	//Pour activer IE0 et END0 pour SS3
	ADC0_IM_R |= 0x0008;	//Mask pour les interupt
	ADC0_ACTSS_R |= 0x0008;	//Active le SS3
	ADC0_PSSI_R = 0x0008;	//Commmence l'échantillonage
	NVIC_EN0_R |= 0x00020000;	//enable
	NVIC_PRI4_R |= 0x0000F000;	//priorité des interruptions
}

void UART_OutChar(char data)
{
	while((UART0_FR_R&0x0020) !=0); // Wait until TXFF is 0
	UART0_DR_R = data;							// affichage du caractère
}

uint8_t statusBouton (void)
{
	uint8_t input = ~GPIO_PORTF_DATA_R&0x11;	//Bit mask pour prendre seulement les donnees des switchs (On doit mettre ~ parce que pour les switchs ON=0 et OFF=1)
	if(input == 0x11) 			//Si les switchs 1 et 2 sont actives
		return 0x03;
	else if(input == 0x10) 	//Si la switch 1 est active
		return 0x01;
	else if(input == 0x01) 	//Si la switch 2 est active
		return 0x02;
	else										//Si aucune switch est active
		return 0x00;
}

void initTimerZero(void)
{
	SYSCTL_RCGCTIMER_R |= 0x00000001;	//Active le timer
	while((SYSCTL_RCGCTIMER_R&0x00000001) == 0)	{};	//Assure que le clock est prete a l'utilisation
	TIMER0_CTL_R &=~0X01;	
	TIMER0_CFG_R =0X0;
	TIMER0_TAMR_R |= 0X02;	//Periodic mode

	TIMER0_TAILR_R = 1814;	//(1/444100)*80000*1000. Périodicité
	TIMER0_CTL_R |= 0X21;	//Activation du TAOTE et de la clock
}
