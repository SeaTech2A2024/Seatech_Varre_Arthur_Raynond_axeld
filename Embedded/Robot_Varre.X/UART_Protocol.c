#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"

unsigned char UartCalculateChecksum(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction prenant entr�e la trame et sa longueur pour calculer le checksum
    unsigned char checksum = 0;

    checksum ^= 0xFE;
    checksum ^= (unsigned char) (msgFunction >> 8);
    checksum ^= (unsigned char) (msgFunction >> 0);
    checksum ^= (unsigned char) (msgPayloadLength >> 8);
    checksum ^= (unsigned char) (msgPayloadLength >> 0);

    for (int i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction d?encodage et d?envoi d?un message
    unsigned char messageUART [msgPayloadLength + 6];
    
    unsigned char pos=0;
    
    messageUART[pos++] = 0xFE;
    messageUART[pos++] = (unsigned char) (msgFunction >> 8);
    messageUART[pos++] = (unsigned char) (msgFunction >> 0);
    messageUART[pos++] = (unsigned char) (msgPayloadLength >> 8);
    messageUART[pos++] = (unsigned char) (msgPayloadLength >> 0);

    for (int i = 0; i < msgPayloadLength; i++) {
        messageUART[pos++] = msgPayload[i];
    }
    messageUART[pos++] += UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
    SendMessage(messageUART, pos);
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c) {
    //Fonction prenant en entr�e un octet et servant � reconstituer les trames
}

void UartProcessDecodedMessage(unsigned char function,
        unsigned char payloadLength, unsigned char* payload) {
    //Fonction appel�e apr�s le d�codage pour ex�cuter l?action
    //correspondant au message re�u
}

 

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
