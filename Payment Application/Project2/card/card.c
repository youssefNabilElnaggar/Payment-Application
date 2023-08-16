#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "stdio.h"
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

#ifndef TEST_MODE
	printf("Hello user, please enter your name: \n");
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}

	fgets(cardData->cardHolderName, sizeof(cardData->cardHolderName), stdin);

	if (cardData->cardHolderName[strlen(cardData->cardHolderName) - 1] == '\n') {
		cardData->cardHolderName[strlen(cardData->cardHolderName) - 1] = '\0';
	}

#endif

	if ((strlen(cardData->cardHolderName) > 10) && (strlen(cardData->cardHolderName) <= 24)) {
		for (int i = 0; i < strlen(cardData->cardHolderName); i++) {
			if ((cardData->cardHolderName[i] >= 65 && cardData->cardHolderName[i] <= 90) || (cardData->cardHolderName[i] >= 97 && cardData->cardHolderName[i] <= 122) || (cardData->cardHolderName[i] == ' ') || (cardData->cardHolderName[i] == '\0')) {
				continue;
			}
			else {
				return WRONG_NAME;
			}
		}
		return CARD_OK;
	}
	else {
		return WRONG_NAME;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
#ifndef TEST_MODE

	printf("Hello %s, please enter your pan: \n", cardData->cardHolderName);
	fgets(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), stdin);

#endif // !TEST_MODE
	
		if (strlen(cardData->primaryAccountNumber) == 17) {
			for (int i = 0; i < 16; i++) {
				if (!isdigit(cardData->primaryAccountNumber[i])) {
					return WRONG_PAN;
				}
			}
			return CARD_OK;
		}
		else {
			return WRONG_PAN;
		}
	return WRONG_PAN;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
#ifndef TEST_MODE

		printf("enter the expiration date\n\tlike:mm/yy\n\t");
		fgets(cardData->cardExpirationDate, 6, stdin);
#endif // !TEST_MODE
		if ((cardData->cardExpirationDate[0] > 49) || (cardData->cardExpirationDate[2] != 47) || (cardData->cardExpirationDate[3] < 50) || (cardData->cardExpirationDate[4] < 51)){
			return WRONG_EXP_DATE;
		}
		else if ((cardData->cardExpirationDate[0] == 49) && (cardData->cardExpirationDate[1] > 50)) {
			return WRONG_EXP_DATE;
		}
		else if ((cardData->cardExpirationDate[4] == 51) && (cardData->cardExpirationDate[3] == 50) && (cardData->cardExpirationDate[0] == 48) && (cardData->cardExpirationDate[1] < 56)) {
			return WRONG_EXP_DATE;
		}
		else if (!isdigit(cardData->cardExpirationDate[0]) || (!isdigit(cardData->cardExpirationDate[1])) || (!isdigit(cardData->cardExpirationDate[3])) || (!isdigit(cardData->cardExpirationDate[4]))) {
			return WRONG_EXP_DATE;
		}
		else {
			return CARD_OK;
		}
}


void getCardHolderNameTest(void) {
	ST_cardData_t userTest;
	EN_cardError_t err;

	strcpy_s(userTest.cardHolderName, sizeof(userTest.cardHolderName), "youssef nabil elnagar");
	err = getCardHolderName(&userTest);
	printf("Test case 1\n");
	printf("Input data: youssef nabil elnagar \n");
	printf("Expected result: %d\n",CARD_OK);
	printf("Actual result: %d\n\n", err);

	strcpy_s(userTest.cardHolderName, sizeof(userTest.cardHolderName), "youssef nabil ");
	err = getCardHolderName(&userTest);
	printf("Test case 2\n");
	printf("Input data: youssef nabil \n");
	printf("Expected result: %d\n", WRONG_NAME);
	printf("Actual result: %d\n\n", err);

	strcpy_s(userTest.cardHolderName, sizeof(userTest.cardHolderName), "youssef nabil123");
	err = getCardHolderName(&userTest);
	printf("Test case 3\n");
	printf("Input data: youssef nabil123 \n");
	printf("Expected result: %d\n", WRONG_NAME);
	printf("Actual result: %d\n\n", err);

	strcpy_s(userTest.cardHolderName, sizeof(userTest.cardHolderName), " ");
	err = getCardHolderName(&userTest);
	printf("Test case 4\n");
	printf("Input data: NULL \n");
	printf("Expected result: %d\n", WRONG_NAME);
	printf("Actual result: %d\n\n", err);
}

void getCardPANTest(void) {
	ST_cardData_t userTest;
	EN_cardError_t err;

	strcpy_s(userTest.primaryAccountNumber, sizeof(userTest.primaryAccountNumber), "1234567890123456");
	err = getCardPAN(&userTest);
	printf("Test case 1\n");
	printf("Input data: 1234567980123456\n");
 	printf("Expected result:%d\n",CARD_OK);
	printf("Actual result:%d\n",CARD_OK);

	strcpy_s(userTest.primaryAccountNumber, sizeof(userTest.primaryAccountNumber), "1234567890");
	err = getCardPAN(&userTest);
	printf("Test case 2\n");
	printf("Input data: 1234567980\n");
	printf("Expected result:%d\n", WRONG_PAN);
	printf("Actual result:%d\n", WRONG_PAN);

	strcpy_s(userTest.primaryAccountNumber, sizeof(userTest.primaryAccountNumber), "12345678901234567");
	err = getCardPAN(&userTest);
	printf("Test case 3\n");
	printf("Input data: 12345679801234567\n");
	printf("Expected result:%d\n", WRONG_PAN);
	printf("Actual result:%d\n", WRONG_PAN);

	strcpy_s(userTest.primaryAccountNumber, sizeof(userTest.primaryAccountNumber), "123456789m123456");
	err = getCardPAN(&userTest);
	printf("Test case 4\n");
	printf("Input data: 123456798m123456\n");
	printf("Expected result:%d\n", WRONG_PAN);
	printf("Actual result: %d\n", WRONG_PAN);

	strcpy_s(userTest.primaryAccountNumber, sizeof(userTest.primaryAccountNumber), "12345678901234 6");
	err = getCardPAN(&userTest);
	printf("Test case 5\n");
	printf("Input data: 12345679801234 6\n");
	printf("Expected result:%d\n", WRONG_PAN);
	printf("Actual result:%d\n", WRONG_PAN);

}

void getCardExpiryDateTest(void) {
	ST_cardData_t userTest;
	EN_cardError_t err;

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "12/23");
	err = getCardExpiryDate(&userTest);
	printf("Test case 1\n");
	printf("Input data: 12/23\n");
	printf("Expected result:%d\n", CARD_OK);
	printf("Actual result:%d\n", CARD_OK);

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "12*23");
	err = getCardExpiryDate(&userTest);
	printf("Test case 2\n");
	printf("Input data: 12*23\n");
	printf("Expected result:%d\n", WRONG_EXP_DATE);
	printf("Actual result:%d\n", WRONG_EXP_DATE);

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "12/20");
	err = getCardExpiryDate(&userTest);
	printf("Test case 3\n");
	printf("Input data: 12/20\n");
	printf("Expected result:%d\n", WRONG_EXP_DATE);
	printf("Actual result:%d\n", WRONG_EXP_DATE);

	strcpy_s(userTest.cardExpirationDate, sizeof(userTest.cardExpirationDate), "15/23");
	err = getCardExpiryDate(&userTest);
	printf("Test case 4\n");
	printf("Input data: 15/23\n");
	printf("Expected result:%d\n", WRONG_EXP_DATE);
	printf("Actual result:%d\n", WRONG_EXP_DATE);
}

