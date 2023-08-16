#define _CRT_SECURE_NO_WARNINGS


#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "stdio.h"
#include "server.h"
#include "../card/card.h"
#include "../terminal/terminal.h"

EN_transState_t receiveTransactionData(ST_transaction_t* transData) {
	if ((transData->cardHolderData.primaryAccountNumber == NULL) ||
		(transData->cardHolderData.primaryAccountNumber == NULL)) {
		return INTERNAL_SERVER_ERROR;
	}
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountReference) {
	for (int indexInAccountReference = 0; indexInAccountReference < 10; indexInAccountReference++) {
		if (strncmp(cardData->primaryAccountNumber, accountReference[indexInAccountReference].primaryAccountNumber, 15) == 0) {
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

int getIndexOfAccounts(int indexInAccountReference, ST_cardData_t* cardData, ST_accountsDB_t* accountReference) {
	for (indexInAccountReference = 0; indexInAccountReference < 10; indexInAccountReference++) {
		if (strncmp(cardData->primaryAccountNumber, accountReference[indexInAccountReference].primaryAccountNumber, 16) == 0) {
			return indexInAccountReference;
		}
	}
}

EN_serverError_t isBlockedAccount(int indexInAccountReference, ST_accountsDB_t* accountReference) {
	if (accountReference[indexInAccountReference].state != RUNNING) {
		return BLOCKED_ACCOUNT;
	}
	else {
		return SERVER_OK;
	}
}

EN_serverError_t isAmountAvailable(int indexInAccountReference, float       tempAmount, ST_accountsDB_t* accountRefrence) {
	if ((tempAmount > accountRefrence[indexInAccountReference].balance)) {
		return LOW_BALANCE;
	}
	else {
		accountRefrence[indexInAccountReference].balance -= tempAmount;
		return SERVER_OK;
	}
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	//transData->transactionSequenceNumber;
	printf("####################################\n");
	printf("Transaction Sequence Number: %d\n", transData->transactionSequenceNumber);
	printf("Transaction Date: %s\n", transData->terminalData.transactionDate);
	printf("Transaction amount: %.02f\n", transData->terminalData.transAmount);
	printf("transaction states: ");
	switch (transData->transState)
	{
	case 0:
		printf("APPROVED \n");
		break;
	case 1:
		printf("Declined due to insffecient fund\n");
		break;
	case 2:
		printf("Declined stolen card\n");
		break;
	case 3:
		printf("fraud card\n");
		break;
	case 4:
		printf("internal server error\n");
		break;
	case 5:
		printf("DECLINED\n");
		break;
	case 6:
		printf("EXCEED MAXIMUM\n");
		break;
	case 7:
		printf("Invaled amount\n");
		break;
	default:

		break;
	}
	printf("terminal Max Amount: %.02f\n", transData->terminalData.maxTransAmount);
	printf("Cardholder Name: %s\n", transData->cardHolderData.cardHolderName);
	printf("PAN: %s", transData->cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n", transData->cardHolderData.cardExpirationDate);
	printf("######################################\n\n");
	return SERVER_OK;
}

void listSavedTransactions(ST_transaction_t* transData)
{
	saveTransaction(transData);
}

void isBlockedAccountTest(void)
{
	EN_serverError_t error;
	ST_accountsDB_t accountReference[10] = {
		{.balance = 10000,.primaryAccountNumber = "1111222233334444",.state = RUNNING},
		{.balance = 5000,.primaryAccountNumber = "1111222233335555",.state = RUNNING},
		{.balance = 1000,.primaryAccountNumber = "9999888877776666",.state = RUNNING},
		{.balance = 10000,.primaryAccountNumber = "1234567891234560",.state = RUNNING},
		{.balance = 100000,.primaryAccountNumber = "1111222233336666",.state = BLOCKED},
		{.balance = 100000,.primaryAccountNumber = "1111222233331111",.state = RUNNING},
		{.balance = 10000,.primaryAccountNumber = "2222211111222222",.state = RUNNING},
		{.balance = 1000,.primaryAccountNumber = "5555555555555555",.state = RUNNING},
		{.balance = 100,.primaryAccountNumber = "1111111111111111",.state = BLOCKED},
		{.balance = 10,.primaryAccountNumber = "3333333333333333",.state = RUNNING}
	}; printf("checking if account is blocked\n");
	for (int indexInAccountReference = 1; indexInAccountReference < 4; indexInAccountReference++) {
		error = isBlockedAccount(indexInAccountReference, &accountReference[indexInAccountReference]);

		printf("case %d\n", indexInAccountReference);
		printf("input : %d\n", accountReference[indexInAccountReference].state);
		printf("expected result: %d\n", RUNNING);
		printf("Actual result: %d\n", accountReference[indexInAccountReference].state);
	}
	printf("\n\n");
}

void isValidAccountTest(void)
{
	ST_accountsDB_t accountReference[10] = {
		{.balance = 10000,.primaryAccountNumber = "1111222233334444",.state = RUNNING},
		{.balance = 5000,.primaryAccountNumber = "1111222233335555",.state = RUNNING},
		{.balance = 1000,.primaryAccountNumber = "9999888877776666",.state = RUNNING},
		{.balance = 10000,.primaryAccountNumber = "1234567891234560",.state = RUNNING},
		{.balance = 100000,.primaryAccountNumber = "1111222233336666",.state = BLOCKED},
		{.balance = 100000,.primaryAccountNumber = "1111222233334444",.state = RUNNING},
		{.balance = 10000,.primaryAccountNumber = "2222211111222222",.state = RUNNING},
		{.balance = 1000,.primaryAccountNumber = "5555555555555555",.state = RUNNING},
		{.balance = 100,.primaryAccountNumber = "1111111111111111",.state = BLOCKED},
		{.balance = 10,.primaryAccountNumber = "3333333333333333",.state = RUNNING}
	};
	printf("checking if it is a valid account\n");
	EN_serverError_t error;
	ST_cardData_t TestUser;

	printf("case 1\n");
	printf("input: 1111222233336666\n");
	strcpy(TestUser.primaryAccountNumber, "1111222233336666");
	error = isValidAccount(&TestUser, &accountReference);
	printf("expected result: %d\n", SERVER_OK);
	printf("Actual result; %d\n\n", error);


	printf("case 2\n");
	printf("input: 1111222233334444\n");
	strcpy(TestUser.primaryAccountNumber, "1111222233334444");
	error = isValidAccount(&TestUser, &accountReference);
	printf("expected result: %d\n", SERVER_OK);
	printf("Actual result; %d\n\n", error);


	printf("case 3\n");
	printf("input: 1111222233335555\n");
	strcpy(TestUser.primaryAccountNumber, "1111222233335555");
	error = isValidAccount(&TestUser, &accountReference);
	printf("expected result: %d\n", ACCOUNT_NOT_FOUND);
	printf("Actual result; %d\n\n", error);


}


