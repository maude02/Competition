#ifndef __DELAY_H__
#define __DELAY_H__


#include<stdint.h>



/**
 *	\brief	Cette fonction sert a bruler des instructions dans le code. 
 *					C'est un exemple de fonction en assembleur, mais en general, ce n'est pas
 *					la facon de generer un delai.
 *
 *	\param	uint32_t time_ms [in] : Le temps que sera arrete le processeur, en ms 
 *
 *	\return void
 */
void delay (uint32_t time_ms);
int rotate(int grosseVariable);



#endif
