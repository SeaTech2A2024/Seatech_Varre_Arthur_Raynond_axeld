#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include  "IO.h"
#include "timer.h"
#include "Robot.h"
#include "PWM.h"
int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();
//initialisation du timer23
InitTimer23();
//initialisation du timer1
InitTimer1();
// initialisation des moteur
InitPWM();
PWMSetSpeed(-20,0);
/****************************************************************************************************/
// Configuration des entrées sorties
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
