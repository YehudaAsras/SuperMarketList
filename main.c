#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "Hfolder/General.h"
#include "Hfolder/Supermarket.h"
#include "Hfolder/ShoppingCart.h"
#include <unistd.h>

int main()
{
	SuperMarket	market;

	srand((unsigned int)time(NULL));
    
    if (loadSuperMarketData(&market, "/Users/yehudaasras/Downloads/Prod.bin", "/Users/yehudaasras/Downloads/Cust.txt")) {
        printf("Supermarket data loaded successfully from files.\n");
    } else {
        printf("No existing data found. Initializing a new supermarket.\n");
        if (!initSuperMarket(&market)) {
            printf("Error initializing Super Market.\n");
            return 0;
       }
    }

	int option;
	int stop = 0;
	
	do
	{
		option = menu();        
		switch (option)
		{
            case eShowSuperMarket:
                printSuperMarket(&market);
                break;

            case eAddProduct:
                if (!addProduct(&market))
                    printf("Error adding product\n");
                break;

            case eAddCustomer:
                if (!addCustomer(&market))
                    printf("Error adding customer\n");
                break;

            case eCustomerDoShopping:
                if(!doShopping(&market))
                    printf("Error in shopping\n");
                break;
           
            case ePrintCart:
                doPrintCart(&market);
                break;

            case eCustomerManageShoppingCart:
                if(!manageShoppingCart(&market))
                    printf("Error in shopping cart managment\n");
                break;

            case ePrintProductByType:
                printProductByType(&market);
                break;
                    
            case eSortSoperMarket:
                    sortSoperMarket(&market);
                    break;

            case eSearchProduct:
                    searchProduct(&market);
                    break;
                    
            case EXIT:
                if (!saveSuperMarketData(&market, "/Users/yehudaasras/Downloads/Prod.bin", "/Users/yehudaasras/Downloads/Cust.txt"))
                {
                    printf("Failed to save supermarket data!\n");
                }
                printf("Saving products to: %s\n", "Prod.bin");
                printf("Saving customers to: %s\n", "Cust.txt");
                printf("Bye bye\n");
                stop = 1;
                break;

            default:
                printf("Wrong option\n");
                break;
		}
	} while (!stop);


	freeMarket(&market);
	
	return 1;
}

int menu()
{
	int option;
	printf("\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

