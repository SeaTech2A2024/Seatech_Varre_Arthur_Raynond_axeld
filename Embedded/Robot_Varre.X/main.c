#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include  "IO.h"
#include "timer.h"
#include "Robot.h"
#include "PWM.h"
#include "adc.h"

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();
    
    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
        
    //initialisation du timer23
    InitTimer23();
    //initialisation du timer1
    InitTimer1();
    // initialisation des moteur
    InitPWM();
    // initialiser ADC
    InitADC1();
    ADCGetResult();

    PWMSetSpeed(20, 1);
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        if (ADCIsConversionFinished==1){
            ADCValue0=ADCResult[0];
            ADCValue1=ADCResult[1];
            ADCValue2=ADCResult[2];
        }
    }

} // fin main
