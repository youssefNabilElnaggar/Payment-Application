#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <string.h>
#include "stdio.h"
#include "app.h"
#include <dos.h>
#include "../card/card.h"
#include "../terminal/terminal.h"
#include "../server/server.h"

void appstart(void) {
EN_cardError_t err;
ST_cardData_t TestUser;

ST_terminalData_t termData;
EN_terminalError_t termErr;

ST_transaction_t transData[10];
EN_transState_t statesErr;

EN_serverError_t serverErr;

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
EN_accountState_t accountErr;

#ifndef TEST_MODE
	int op;
	int i = -1,
		numberOftrans =0;
	float maxAmount;
	printf("enter max Amount \n");
	scanf("%f", &maxAmount);


	while (1)
	{
		numberOftrans++;

		printf("\npress 1 for transction\n");
		printf("press 2 for list saved transaction\n");
		printf("press 3 for test\n");

		scanf("%d", &op);

		switch (op) {
		case 1:
			i++;

			transData[i].transactionSequenceNumber = numberOftrans;
			transData[i].transState = APPROVED;

			termData.maxTransAmount = maxAmount;
			termData.transAmount = 0;

			getTransactionDate(&termData);

			err = getCardHolderName(&TestUser);
			if (err != CARD_OK) {
				printf("error in holder name\n");
				strcpy(TestUser.cardHolderName, "error");
				transData[i].transState = DECLINED;
			}

			err = getCardPAN(&TestUser);
			if (err != CARD_OK) {
				printf("error in PAN number\n");
				strcpy(TestUser.primaryAccountNumber, "error\n");
				transData[i].transState = DECLINED;
			}

			err = getCardExpiryDate(&TestUser);
			if (err != CARD_OK) {
				printf("error in Date\n");
				strcpy(TestUser.cardExpirationDate, "error");
				transData[i].transState = DECLINED;
			}

			transData[i].cardHolderData = TestUser;

			if (err != CARD_OK) {
				transData[i].transState = DECLINED;
				transData[i].terminalData = termData;
				saveTransaction(&transData[i]);
				break;
			}

			termErr = isCardExpired(&TestUser, &termData);
			if (termErr != TERMINAL_OK) {
				printf("error in card expiry date\n");
			}

			termErr = setMaxAmount(&termData, maxAmount);
			if (termErr != TERMINAL_OK) {
				printf("invalid max Amount\n");
				transData[i].transState = INVALED_MAX;
				termData.maxTransAmount = 0;
			}

			termErr = getTransactionAmount(&termData);
			if (termErr != TERMINAL_OK) {
				printf("error Amount\n");
				transData[i].transState = INVALID_AMOUNT;
			}

			termErr = isBelowMaxAmount(&termData);
			if (termErr != TERMINAL_OK) {
				printf("Exceed maximam Amount\n");
				transData[i].transState = EXCEED_MAX;
			}

			if (transData[i].transState != APPROVED) {
				transData[i].terminalData = termData;
				saveTransaction(&transData[i]);
				break;
			}

			int index = 0;
			int indexInAccountReference = 0;

			serverErr = isValidAccount(&TestUser, &accountReference);
			if (serverErr != SERVER_OK) {
				printf("invalid account\n");
				transData[i].transState = INTERNAL_SERVER_ERROR;
				transData[i].terminalData = termData;
				saveTransaction(&transData[i]);
				break;
			}

			indexInAccountReference = getIndexOfAccounts(index, &TestUser, &accountReference);

			serverErr = isBlockedAccount(indexInAccountReference, &accountReference);
			if (serverErr != SERVER_OK) {
				printf("account is blocked\n");
				transData[i].transState = DECLINED_STOLEN_CARD;
			}
			else {
				serverErr = isAmountAvailable(indexInAccountReference, termData.transAmount, &accountReference);
				if (serverErr != SERVER_OK) {
					printf("No balance\n");
					transData[i].transState = DECLINED;
				}
			}

			transData[i].terminalData = termData;

			statesErr = receiveTransactionData(&transData[i]);
			if (statesErr != APPROVED) {
				printf("Internal server error \n");
				transData[i].transState = INTERNAL_SERVER_ERROR;
			}

				 saveTransaction(&transData[i]);

			break;
		case 2:
			 
			
			for (i = 0; i< numberOftrans-1; i++) {
				listSavedTransactions(&transData[i]);
			}
			break;

		case 3:
			getCardHolderNameTest();
			getCardPANTest();
			getCardExpiryDateTest();

			isCardExpiredTest();
			getTransactionAmountTest();
			isBelowMaxAmountTest();
			setMaxAmountTest();

		//  isBlockedAccountTest();
		//	receiveTransactionDataTest();
		//	isValidAccountTest();


			break;
		default:
			break;
		}
	}
}
#endif // TEST_MODE