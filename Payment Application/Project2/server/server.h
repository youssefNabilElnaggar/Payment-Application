#ifndef SERVER_H
#define SERVER_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include "../card/card.h"
#include "../terminal/terminal.h"
#include "../application/app.h"
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "stdio.h"

typedef enum EN_transState_t
{
    APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR, DECLINED, EXCEED_MAX , INVALED_MAX, INVALED_AMOUNT
}EN_transState_t;

typedef struct ST_transaction_t
{
    ST_cardData_t cardHolderData;
    ST_terminalData_t terminalData;
    EN_transState_t transState;
    uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum EN_serverError_t
{
    SERVER_OK=19, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT
}EN_serverError_t;

typedef enum EN_accountState_t
{
    RUNNING,
    BLOCKED
}EN_accountState_t;

typedef struct ST_accountsDB_t
{
    float balance;
    EN_accountState_t state;
    uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

EN_transState_t receiveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountReference);
int getIndexOfAccounts(int indexInAccountReference, ST_cardData_t* cardData, ST_accountsDB_t* accountReference);
EN_serverError_t isBlockedAccount(int indexInAccountReference,ST_accountsDB_t* accountReference);
EN_serverError_t isAmountAvailable(int indexInAccountReference,float tempAmount, ST_accountsDB_t* accountReference);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
void listSavedTransactions(ST_transaction_t* transData);


/*Test Functions*/
void isBlockedAccountTest(void);
void receiveTransactionDataTest(void);
void isValidAccountTest(void);

#endif  
//SERVER_H