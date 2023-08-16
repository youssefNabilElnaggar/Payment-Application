#ifndef CARD_H
#define CARD_H

//#define TEST_MODE


#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "stdio.h"
#include <dos.h>

typedef struct ST_cardData_t
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
    CARD_OK=1, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);

EN_cardError_t getCardPAN(ST_cardData_t* cardData);

EN_cardError_t cmpCardExpiryDate(ST_cardData_t* cardData);


/* Test Functions */
void getCardHolderNameTest(void);
void getCardExpiryDateTest(void);
void getCardPANTest(void);

#endif //CARD_H