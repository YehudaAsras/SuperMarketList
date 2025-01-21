#pragma once
#include "ShoppingCart.h"

#define CUSTOMER_ID_LENGTH 9
#define NAMES_SEP " "
#define NAME_PARTS_SEP " - "

typedef enum { REGULAR_CUSTOMER, CLUB_MEMBER } CustomerType;
typedef struct
{
	char*			id;
	char*			name;
	ShoppingCart*	pCart;
    CustomerType    type;
}Customer;


int	        initCustomer(Customer* pCustomer);
int         initRegularCustomer(Customer* pCustomer);
void	    printCustomer(const Customer* pCustomer);

int		    isCustomerIdValid(const char* id);

int		    isCustomer(const Customer* pCust, const char* name);
int		    isCustomerById(const Customer* pCust, const char* id);
//int		isCustomersNameValid(const char* name);
void	    getNamePart(char* part, const char* msg);
void	    upperLowerCustomerName(char* name);
//char*	combineFirstLast(char*  firstName, char* lastName);
char*       combineFirstLast(char** parts);
void	    pay(Customer* pCustomer);
void	    cancelShopping(Customer* pCustomer);

void	    freeCustomer(Customer* pCust);



