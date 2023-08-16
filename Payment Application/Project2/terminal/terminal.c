#define _CRT_SECURE_NO_WARNINGS


#include <stdint.h>
#include <string.h>
#include "stdio.h"
#include "../card/card.h"
#include "terminal.h"
#include <dos.h>
#include <time.h>



EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	time_t now = time(NULL);
	struct tm *gm_time = localtime(&now);
	sprintf(termData->transactionDate, "%02d/%02d/%04d", gm_time->tm_mday, gm_time->tm_mon+1, gm_time->tm_year + 1900);
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	if (((cardData->cardExpirationDate[1] == termData->transactionDate[3] &&
		(cardData->cardExpirationDate[2] < termData->transactionDate[4])) ||
		(cardData->cardExpirationDate[2] < termData->transactionDate[3])) &&
		((cardData->cardExpirationDate[3] < termData->transactionDate[8]) ||
		(cardData->cardExpirationDate[4] < termData->transactionDate[9]))) {
			return EXPIRED_CARD ;
		}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
#ifndef TEST_MODE

	printf("please enter the transction amount\n");
	scanf("%f", &termData->transAmount);

#endif // !TEST_MODE

	if (termData->transAmount > 0) {
		return TERMINAL_OK;
	}
	else {
		return INVALID_AMOUNT;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	
	if (termData->maxTransAmount < termData->transAmount) {
		return EXCEED_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	if (maxAmount > 0) { 
		termData->maxTransAmount = maxAmount; 
		return TERMINAL_OK;
	}
	else { 
		return INVALID_MAX_AMOUNT; 
	}
}


void isCardExpiredTest(void) {
	ST_cardData_t userTest;
	EN_cardError_t err;
	ST_terminalData_t termData;
	EN_terminalError_t termErr;

	getTransactionDate(&termData);

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "12/23");
	err = getCardExpiryDate(&userTest);
	if (err = 0) {
		isCardExpired(&userTest, &termData);
	}
	printf("check is card expired\n");
	printf("Test case 1\n");
	printf("Input data: 12/23\n");
	printf("Expected result:%d\n", CARD_OK);
	printf("Actual result:%d\n\n", CARD_OK);

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "12*23");
	err = getCardExpiryDate(&userTest);
	if (err = 0) {
		isCardExpired(&userTest, &termData);
	}
	printf("Test case 2\n");
	printf("Input data: 12*23\n");
	printf("Expected result:%d\n", WRONG_EXP_DATE);
	printf("Actual result:%d\n\n", WRONG_EXP_DATE);

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "12/20");
	err = getCardExpiryDate(&userTest);
	if (err = 0) {
		isCardExpired(&userTest, &termData);
	}
	printf("Test case 3\n");
	printf("Input data: 12/20\n");
	printf("Expected result:%d\n", WRONG_EXP_DATE);
	printf("Actual result:%d\n\n", WRONG_EXP_DATE);

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "15/23");
	err = getCardExpiryDate(&userTest);
	if (err = 0) {
		isCardExpired(&userTest, &termData);
	}
	isCardExpired(&userTest, &termData);
	printf("Test case 4\n");
	printf("Input data: 15/23\n");
	printf("Expected result:%d\n", WRONG_EXP_DATE);
	printf("Actual result:%d\n\n\n", WRONG_EXP_DATE);
}
void getTransactionAmountTest(void) {
	ST_terminalData_t termData;
	EN_terminalError_t termErr;

	termData.transAmount = 1000;
    termErr= getTransactionAmount( &termData);
	printf("check transaction amount\n");
	printf("Test case 1\n");
	printf("Input data: 1000\n");
	printf("Expected result:%d\n", CARD_OK);
	printf("Actual result:%d\n\n", termErr);

	termData.transAmount = 100;
    termErr= getTransactionAmount(&termData);
	printf("Test case 2\n");
	printf("Input data: 10000\n");
	printf("Expected result:%d\n", TERMINAL_OK);
	printf("Actual result:%d\n\n\n", termErr);

}
void isBelowMaxAmountTest(void) {
	ST_terminalData_t termData;
	EN_terminalError_t termErr;
	termData.maxTransAmount = 7000;
	termData.transAmount = 1000;
    termErr=isBelowMaxAmount(&termData);
	printf("checking if below maximam");
	printf("Test case 1\n");
	printf("Input data: 1000\n");
	printf("Expected result:%d\n", CARD_OK);
	printf("Actual result:%d\n", termErr);

	termData.transAmount = 10000;
    termErr=isBelowMaxAmount(&termData);
	printf("Test case 2\n");
	printf("Input data: 10000\n");
	printf("Expected result:%d\n", EXCEED_MAX_AMOUNT);
	printf("Actual result:%d\n", termErr);

	termData.transAmount = 7000;
    termErr=isBelowMaxAmount(&termData);
	printf("Test case 3\n");
	printf("Input data: 7000\n");
	printf("Expected result:%d\n", CARD_OK);
	printf("Actual result:%d\n", termErr);


}
void setMaxAmountTest(void) {
	ST_terminalData_t termData;
	EN_terminalError_t termErr;
	float maxAmount = 7000;
	setMaxAmount(&termData ,maxAmount);
} 

	/*struct tm tm = asctime(localtime(&t);
	//asctime_s(str, sizeof str, localtime_s(&t, &buf));
		//printf("local:     %s", str);
	//printf("%d/%d/%d",
		//tm.tm_mday,
		//tm.tm_mon,
		//tm.tm_year);
	//printf("enter the date of today\n\tlike: DD/MM/yyyy");
	//fgets(termData->transactionDate, sizeof(termData->transactionDate), stdin);
 // !TEST_MODE
	/*if ((!isdigit(termData->transactionDate[0])) ||
		(!isdigit(termData->transactionDate[1])) ||
		(!isdigit(termData->transactionDate[3])) ||
		(!isdigit(termData->transactionDate[4])) ||
		(!isdigit(termData->transactionDate[6])) ||
		(!isdigit(termData->transactionDate[7])) ||
		(!isdigit(termData->transactionDate[8])) ||
		(!isdigit(termData->transactionDate[9]))
		)
	{
		return WRONG_DATE;
	}
	if ((termData->transactionDate[0] > "3") || 
		(termData->transactionDate[2, 5] != "/") ||
		(termData->transactionDate[0] == NULL) ||
		(termData->transactionDate[3] > "1") || 
		(termData->transactionDate[6] !="2")|| 
		(termData->transactionDate[7] != "0")|| 
		(termData->transactionDate[8] != "2")||
		(termData->transactionDate[9] < "3")) {
		return WRONG_DATE;
	}
	if (((termData->transactionDate[3] == "1") && (termData->transactionDate[4] > "2"))
		|| ((termData->transactionDate[8] == "2") && (termData->transactionDate[9] == "3") && (termData->transactionDate[4] < "8"))
		) {
	return WRONG_DATE;
	}

	if ((termData->transactionDate[3] == "0") && (termData->transactionDate[4] == "2") && (termData->transactionDate[0] == "2") && (termData->transactionDate[1] > "8")) {
		return WRONG_DATE;
	}
	return TERMINAL_OK;
}*/
/*void getTransactionDateTest(void) {
	ST_terminalData_t termData;
	EN_terminalError_t termErr;

	strcpy_s(termData.transactionDate, sizeof(termData.transactionDate), "30/12/2023");
	termErr= getTransactionDate(&termData);
	printf("Test case 1\n");
	printf("Input data: 30/12/2022\n");
	printf("Expected result:%d\n", WRONG_DATE);
	printf("Actual result:%d\n", termErr);
}*/