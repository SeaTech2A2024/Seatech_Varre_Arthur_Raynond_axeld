#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include  "IO.h"
#include "timer.h"

int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();
//initialisation du timer23
InitTimer23();
//initialisation du timer1
InitTimer1();
/****************************************************************************************************/
// Configuration des entr�es sorties
/****************************************************************************************************/
InitIO();

LED_BLANCHE = 1;
LED_BLEUE = 1;
LED_ORANGE = 1;

/****************************************************************************************************/
// Boucle Principale
/****************************************************************************************************/
while(1){
    }

} // fin main