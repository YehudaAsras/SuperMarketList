#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Hfolder/Customer.h"
#include "../Hfolder/General.h"
#include "../Hfolder/ClubMenber.h"

int initCustomer(Customer* pCustomer) {
    char isMember;

    do {
        printf("Is the customer a club member? (y/Y for yes, n/N for no): ");
        scanf(" %c", &isMember);
    } while (tolower(isMember) != 'y' && tolower(isMember) != 'n'); // Ensure valid input

    if (tolower(isMember) == 'y') {
        ClubMember* pMember = (ClubMember*)pCustomer;
        return initClubMember(pMember);
    }

    return initRegularCustomer(pCustomer);
}

int initRegularCustomer(Customer* pCustomer) {
    char firstName[MAX_STR_LEN];
    char lastName[MAX_STR_LEN];
    char* parts[2] = { firstName, lastName };
    char* msgParts[2] = { "Enter customer first name: ", "Enter customer last name: " };

    // Get and process first and last name
    for (int i = 0; i < 2; i++) {
        getNamePart(parts[i], msgParts[i]);
        upperLowerCustomerName(parts[i]);
    }

    // Combine the first and last name into a full name
    char* combineName = combineFirstLast(parts);
    if (!combineName)
        return 0;

    pCustomer->name = combineName;

    // Get the customer ID and validate it
    char msg[MAX_STR_LEN];
    sprintf(msg, "ID should be %d digits (For example: 123456789): ", CUSTOMER_ID_LENGTH);
    do {
        pCustomer->id = getStrExactLength(msg);
        if (!pCustomer->id)
            return 0;
    } while (!isCustomerIdValid(pCustomer->id));

    pCustomer->pCart = NULL;
    pCustomer->type = REGULAR_CUSTOMER;
    return 1;
}

void getNamePart(char* part, const char* msg)
{
	int ok = 0;
	do {
		getsStrFixSize(part, MAX_STR_LEN, msg);
		if (checkEmptyString(part))
			printf("Name cound not be empty\n");
		else if (!checkAlphaSpaceStr(part))
			printf("Name should contain only letters\n");
		else
			ok = 1;
	} while (!ok);
}

void	upperLowerCustomerName(char* name)
{
	toLowerStr(name);
	while (isspace(*name))
		name++;

	*name = toupper(*name);
}

char* combineFirstLast(char** parts)
{
	char temp[MAX_STR_LEN * 2] = { 0 };
	char** pNameParts = NULL;
	int count = 0;
	size_t totalLength = 0;

	for (int i = 0; i < 2; i++)
	{
		pNameParts = splitCharsToWords(parts[i], &count, NAMES_SEP, &totalLength);
		if (!pNameParts)
			return NULL;

		for (int j = 0; j < count; j++)
		{
			strcat(temp, pNameParts[j]);
			strcat(temp, " ");
			free(pNameParts[j]);
		}
		free(pNameParts);
		pNameParts = NULL;

		if (i == 0)
			strcat(temp, "- ");
	}
	temp[strlen(temp) - 1] = '\0';
	
	char* combineName = myStrDup(temp);
	return combineName;
}

void printCustomer(const Customer* pCustomer)
{
	printf("Name: %s\n", pCustomer->name);
	printf("ID: %s\n", pCustomer->id);

	if (pCustomer->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else
	{
		printf("Doing shopping now!!!\n");
	}
}

int isCustomerIdValid(const char* id)
{
	if (strlen(id) != CUSTOMER_ID_LENGTH) return 0;
    
	for (int i = 0; i < CUSTOMER_ID_LENGTH; i++)
	{
		if (!isdigit(id[i])) return 0;
	}
	return 1;
}


void pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	printShoppingCart(pCustomer->pCart);
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}

void cancelShopping(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("!!! --- Purchase was canceled!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}


int	isCustomerById(const Customer* pCust, const char* id)
{
	if (strcmp(pCust->id, id) == 0)
		return 1;
	return 0;
}
//
//
//int isCustomersNameValid(const char* name)
//{
//	int i = 0;
//	while (name[i])
//	{
//		if (!isalpha(name[i]))
//		{
//			printf("Name should contain only letters\n");
//			return 0;
//		}
//		i++;
//	}
//	return 1;
//}

int	isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void freeCustomer(Customer* pCust)
{
	if (pCust->pCart)
		pay(pCust); //will free every thing
	free(pCust->name);
	free(pCust->id);
}
