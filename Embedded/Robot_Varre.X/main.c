#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include  "IO.h"
#include "timer.h"
#include "Robot.h"
#include "PWM.h"
#include "adc.h"
#include "main.h"

unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(-25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //D�termination de la position des obstacles en fonction des t�l�m�tres
    if (robotState.distanceTelemetreDroit < 25 &&
        robotState.distanceTelemetreCentre > 40 &&
        robotState.distanceTelemetreGauche > 25)
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreGauche < 25 &&
        robotState.distanceTelemetreCentre > 40 &&
        robotState.distanceTelemetreDroit > 25)
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 40)//Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreCentre < 40 &&
             robotState.distanceTelemetreDroit <25)
        positionObstacle = OBSTACLE_LEG_DROIT ;
    else if (robotState.distanceTelemetreCentre < 40 &&
             robotState.distanceTelemetreGauche <25)
        positionObstacle = OBSTACLE_LEG_GAUCHE ;
    else if (robotState.distanceTelemetreExGauche < 18 &&
             robotState.distanceTelemetreExDroit > 18 &&
             robotState.distanceTelemetreCentre > 40)
        positionObstacle = OBSTACLE_A_EXGAUCHE;
    else if (robotState.distanceTelemetreExDroit < 18 &&
             robotState.distanceTelemetreExGauche > 18 &&
             robotState.distanceTelemetreCentre > 40)
        positionObstacle = OBSTACLE_A_EXDROITE;
     else if (robotState.distanceTelemetreDroit < 25 &&
            robotState.distanceTelemetreCentre < 40 &&
            robotState.distanceTelemetreGauche < 25 &&
            robotState.distanceTelemetreExDroit < 18 &&
            robotState.distanceTelemetreExGauche < 18) //pas d?obstacle
        positionObstacle = OBSTACLE;
//    else if (robotState.distanceTelemetreCentre > 40 &&
//            robotState.distanceTelemetreExDroit < 30 &&
//            robotState.distanceTelemetreExGauche < 30)
//        positionObstacle = TUNNEL;
//    else if (robotState.distanceTelemetreGauche< 25 &&
//            robotState.distanceTelemetreCentre < 40  &&
//            robotState.distanceTelemetreExGauche < 15)
//        positionObstacle = ANGLEGAUCHE;
//    else if (robotState.distanceTelemetreCentre< 40 &&
//            robotState.distanceTelemetreDroit < 25 &&
//            robotState.distanceTelemetreExDroit < 15)
//        positionObstacle = ANGLEDROIT;
    else if (robotState.distanceTelemetreDroit > 25 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreGauche > 25 &&
            robotState.distanceTelemetreExDroit > 18 &&
            robotState.distanceTelemetreExGauche > 18) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;
    
    
    //D�termination de l?�tat � venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_EXDROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_EXGAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_LEG_GAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
     else if (positionObstacle == OBSTACLE_LEG_DROIT)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
//    else if (positionObstacle == TUNNEL)
//        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == ANGLEDROIT)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == ANGLEGAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
 
    
    //Si l?on n?est pas dans la transition de l?�tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
    
    if (nextStateRobot == stateRobot - 4)
        stateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
}

int ADCValue0;
int ADCValue1;
int ADCValue2;
int ADCValue3;
int ADCValue4;

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entr�es sorties
    /****************************************************************************************************/
    InitIO();

    //initialisation du timer23
    InitTimer23();
    //initialisation du timer1
    InitTimer1();
    //initialisation du timer4
    InitTimer4();
    // initialisation des moteur
    InitPWM();
    // initialiser ADC
    InitADC1();

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExGauche = 34 / volts - 5;
            volts = ((float) result [0])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExDroit = 34 / volts - 5;
            
            
            if (robotState.distanceTelemetreExDroit < 15) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
            if (robotState.distanceTelemetreCentre < 30) {
                LED_BLEUE = 1;
            } else {
                LED_BLEUE = 0;
            }
            if (robotState.distanceTelemetreExGauche < 15) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }

        }
    }

} // fin main
