

                AREA    |.text|, CODE, READONLY


CLOCK	EQU		25000000

; Delay Function
;
;	Cette fonction cree un delai de n ms. n est le nombre stocke dans
;	le registre R0. Lorsqu'une fonction C est appelee, par convention, le premier
;	parametre se trouve dans le registre R0.
;
;	Cette fonction a donc un prototype de la forme suivante
;	void delay (uint32_t temps_ms)
				EXPORT delay
delay			PROC
				LDR		R1,=(CLOCK/1000/3)	;R1 = ((Le nombre de coups de clocks par seconde)/1000/(3 instructions par boucle)) = le nombre de boucles par ms
                MUL		R0,R0,R1		;R0 = R0 * R1	-> Le nombre de boucles a faire pour avoir n ms
Loop			SUBS	R0,R0,#1		;R0 = R0 - 1	-> On decremente une fois par boucle
				BNE		Loop			;				-> On retourne au debut de la boucle si la soustraction n'est pas egale a zero
				BX		LR				;				-> On retourne de la fonction
				ENDP
				
				END
