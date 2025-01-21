#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Hfolder/Supermarket.h"
#include "../Hfolder/Product.h"
#include "../Hfolder/Customer.h"
#include "../Hfolder/General.h"
#include "../Hfolder/ShoppingCart.h"
#include "../Hfolder/ClubMenber.h"

int initSuperMarket(SuperMarket* pMarket) {
    pMarket->name = getStrExactLength("Enter market name: ");
    if (!pMarket->name) return 0;

    L_init(&pMarket->customerList);
    L_init(&pMarket->productList);

    pMarket->customerCount = 0;
    pMarket->productCount = 0;
    pMarket->sortOption = eNoSorted;

    return 1;
}

void printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int addProduct(SuperMarket* pMarket)
{
	char answer;
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = NULL;

	printf("\n");
	printf("Adding new product? y/Y: ");
	scanf("%c", &answer);
	getchar();

	if (toupper(answer) == 'Y')
	{
		if (!addNewProduct(pMarket))
		{
			free(pProd);
			return 0;
		}
		return 1;
	}
	else if (pMarket->productCount > 0)
	{
		printf("Do you want to increase the amount of an existing product? y/Y: ");
		scanf("%c", &answer);
		getchar();
		if (toupper(answer) == 'Y')
		{
			printAllProducts(pMarket);
			pProd = getProductFromUser(pMarket, barcode);
			if (pProd != NULL) //This barcode exist in stock
				updateProductCount(pProd);
		}
	}
	else
		return 0;
	
	return 1;
}

int addNewProduct(SuperMarket* pMarket) {
    // Allocate memory for a new Product
    Product* pProd = (Product*)calloc(1, sizeof(Product));
    if (!pProd) return 0;

    // Initialize the product details
    initProductNoBarcode(pProd);

    // Generate a unique barcode for the product
    do {
        generateBarcode(pProd);
    } while (!isBarcodeUnique(pMarket, pProd->barcode)); // Ensure barcode uniqueness

    // Create a new node for the product
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (!newNode) {
        freeProduct(pProd);
        free(pProd);
        return 0;
    }
    newNode->key = pProd;
    newNode->next = NULL;

    // Add the new node at the beginning of the list
    newNode->next = pMarket->productList.head.next; // Point to the current first node
    pMarket->productList.head.next = newNode;       // Update the head to point to the new node

    // Increment the product count
    pMarket->productCount++;
    return 1;
}

int isBarcodeUnique(const SuperMarket* pMarket, const char* barcode) {
    NODE* current = pMarket->productList.head.next; // Skip dummy head
    while (current) {
        Product* pProd = (Product*)current->key;
        if (strcmp(pProd->barcode, barcode) == 0) {
            return 0; // Barcode is not unique
        }
        current = current->next;
    }
    return 1; // Barcode is unique
}

int isCustomerIdUnique(const SuperMarket* pMarket, const char* id) {
    NODE* current = pMarket->customerList.head.next; // Skip dummy head
    while (current) {
        Customer* pCustomer = (Customer*)current->key;
        if (strcmp(pCustomer->id, id) == 0) {
            printf("ID %s is not unique\n", id);
            return 0; // ID is not unique
        }
        current = current->next;
    }
    return 1; // ID is unique
}

int addCustomer(SuperMarket* pMarket)
{
    Customer* pCust = (Customer*)malloc(sizeof(Customer));
    if (!pCust)
    {
        printf("Memory allocation failed for new customer.\n");
        return 0;
    }

    do {
        if (!initCustomer(pCust))
        {
            free(pCust);
            return 0;
        }
    } while (!isCustomerIdUnique(pMarket, pCust->id));

    if (isCustomerInMarket(pMarket, pCust))
    {
        printf("This customer is already in the market.\n");
        freeCustomer(pCust);
        return 0;
    }
  
    // Insert the customer into the linked list
    if (!L_insert(&(pMarket->customerList.head), (DATA)pCust))
    {
        printf("Failed to add customer to the list.\n");
        freeCustomer(pCust);
        return 0;
    }

    // Update customer count
    pMarket->customerCount++;
    return 1;
}

int isCustomerInMarket(SuperMarket* pMarket, Customer* pCust) {
    NODE* current = pMarket->customerList.head.next; // Start from the first actual node (not the dummy head)

    while (current) {
        Customer* existingCustomer = (Customer*)current->key; // Retrieve the customer stored in the current node

        // Check if the names match
        if (strcmp(existingCustomer->name, pCust->name) == 0) {
            return 1; // Customer is in the market
        }

        current = current->next; // Move to the next node in the list
    }

    return 0; // Customer is not in the market
}


int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

    if (pCustomer->pCart == NULL)
    {
        pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
        if (!pCustomer->pCart)
        {
            return 0;
        }
        initCart(pCustomer->pCart);
    }
    
	fillCart(pMarket, pCustomer->pCart);
    
//	if (pCustomer->pCart->count == 0) //did not buy any thing
//	{
//		free(pCustomer->pCart);
//		pCustomer->pCart = NULL;
//	}
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	printShoppingCart(pCustomer->pCart);
	return pCustomer;
}

int	manageShoppingCart(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	char answer;

	if(!pCustomer)
		return 0;

	printf("Do you want to pay for the cart? y/Y, anything else to cancel shopping!\t");
	do {
		scanf("%c", &answer);
	} while (isspace(answer));

	getchar(); //clean the enter

	if (answer == 'y' || answer == 'Y')
		pay(pCustomer);
	else {
		clearCart(pMarket, pCustomer);
		cancelShopping(pCustomer);
	}
	return 1;

}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return NULL;
	}

	return pCustomer;
}

void printAllProducts(const SuperMarket* pMarket) {
    printf("There are %d products\n", pMarket->productCount);
    printf("%-20s %-10s\t", "Name", "Barcode");
    printf("%-20s %-10s %-20s %-15s\n", "Type", "Price", "Count In Stoke", "Expiry Date");
    printf("-------------------------------------------------------------------------------------------------\n");

    generalListFunction(pMarket->productList.head.next, printProductWrapper);
}

//void	printAllProducts(const SuperMarket* pMarket)
//{
//	printf("There are %d products\n", pMarket->productCount);
//	printf("%-20s %-10s\t", "Name", "Barcode");
//	printf("%-20s %-10s %-20s %-15s\n", "Type", "Price", "Count In Stoke", "Expiry Date");
//	printf("-------------------------------------------------------------------------------------------------\n");
//
//    // Start from the first node in the product list
//        NODE* current = pMarket->productList.head.next;
//
//        while (current)
//        {
//            Product* pProduct = (Product*)(current->key);
//            printProduct(pProduct);
//            current = current->next;
//        }
//}

void printAllCustomers(const SuperMarket* pMarket)
{
    if (!pMarket) return;
    
    if (pMarket->customerCount == 0)
    {
            printf("No customers in the supermarket.\n");
            return;
    }

    printf("There are %d listed customers:\n", pMarket->customerCount);
    printf("====================================\n");

    // Start from the first node in the customer list
    NODE* current = pMarket->customerList.head.next;
    int customerCounter = 1;
    
    while (current)
    {
        Customer* pCustomer = (Customer*)(current->key);
        printf("Customer number %d:\n", customerCounter);

        if (pCustomer->type == CLUB_MEMBER) {
            printClubMember((ClubMember*)pCustomer);
        } else {
            printCustomer(pCustomer);
        }
        customerCounter++;
        current = current->next;
        printf("------------------------------------\n");
    }
}

Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char searchTerm[MAX_STR_LEN];
	getsStrFixSize(searchTerm, sizeof(searchTerm), "Who is shopping? Enter customer id\n");

	Customer* pCustomer = FindCustomerById(pMarket, searchTerm);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket, ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count;
		}
	}
}

void clearCart(SuperMarket* pMarket, Customer* pCustomer) {
    if (pCustomer->pCart == NULL)
    {
        return;
    }

    NODE* current = pCustomer->pCart->head.head.next;
    NODE* temp;

    while (current != NULL)
    {
        ShoppingItem* item = (ShoppingItem*)current->key;
        Product* product = getProductByBarcode(pMarket, item->barcode);

        if (product) {
            product->count += item->count;
        }

        temp = current;
        current = current->next;
        free(item);
        free(temp);
    }

    pCustomer->pCart->head.head.next = NULL;
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType type = getProductType();
	int count = 0;
    
    printf("Products of type %s:\n", getProductTypeStr(type));
    printf("%-20s %-10s\t", "Name", "Barcode");
    printf("%-20s %-10s %-20s %-15s\n", "Type", "Price", "Count In Stoke", "Expiry Date");
    printf("-------------------------------------------------------------------------------------------------\n");

    
    // Start from the first node in the product list
    NODE* current = pMarket->productList.head.next;

    while (current)
    {
        Product* pProduct = (Product*)(current->key);
        
        if (pProduct->type == type)
        {
            count++;
            printProduct(pProduct);
        }
        current = current->next;
    }

    // If no products of the selected type were found
    if (count == 0)
    {
        printf("There are no products of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
    }
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBarcodeCode(barcode);
	Product* pProd = getProductByBarcode(pMarket, barcode);
	if (!pProd)
	{
		printf("No such product barcode\n");
		return NULL;
	}
	return pProd;
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBarcodeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
    NODE* current = pMarket->productList.head.next;
    int index = 0;

    while (current)
    {
        Product* pProduct = (Product*)(current->key);
        if (isProduct(pProduct, barcode))
        {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
    // Start from the first node in the product list
    NODE* current = pMarket->productList.head.next;

    while (current)
    {
        Product* pProduct = (Product*)(current->key);
        if (isProduct(pProduct, barcode))
        {
            return pProduct;
        }
        current = current->next;
    }

    return NULL;
}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
    // Start from the first node in the customer list
    NODE* current = pMarket->customerList.head.next;

    while (current)
    {
        Customer* pCustomer = (Customer*)(current->key);
        if (isCustomer(pCustomer, name))
        {
            return pCustomer;
        }
        current = current->next;
    }

    return NULL;
}

Customer* FindCustomerById(SuperMarket* pMarket, const char* id)
{
    // Start from the first node in the customer list
    NODE* current = pMarket->customerList.head.next;

    // Traverse the linked list
    while (current)
    {
        Customer* pCustomer = (Customer*)(current->key);
        
        if (isCustomerById(pCustomer, id))
        {
            return pCustomer;
        }
        current = current->next;
    }

    return NULL;
}

void  sortSoperMarket(SuperMarket* pMarket)
{
    int option;
    
    option = showSortMenu();
    switch (option)
    {
        case eSortByProductName:
            printf("Sorting products by name...\n");
            sortProductListByName(pMarket, &(pMarket->productList));
            break;

        case eSortByProductQuantity:
            printf("Sorting products by quantity...\n");
            sortProductListByQuantity(pMarket, &(pMarket->productList));
            break;

        case eSortByProducPrice:
            printf("Sorting products by price...\n");
            sortProductListByPrice(pMarket, &(pMarket->productList));
            break;

        case EXIT:
            printf("Ok, Good bay");
            break;

        default:
            printf("Wrong option\n");
            break;
    }
}

int showSortMenu(void) {

    int option;
    printf("Please choose a sorting option:\n");
    for (int i = eSortByProductName; i < eNofOption; i++)
    {
        printf("%d - %s\n", i, menuString[i]);
    }
    printf("%d - Quit\n", EXIT);
    scanf("%d", &option);
    
    //clean buffer
    char tav;
    scanf("%c", &tav);
    
    return option;
}

void sortProductListByName(SuperMarket* pMarket, LIST* pList) {
    if (!pList || !pList->head.next) return;

    NODE* current = pList->head.next;
    int numberOfProduct = pMarket->productCount;

    // Create an array of pointers to the products
    Product** productArray = (Product**)malloc(numberOfProduct * sizeof(Product*));
    if (!productArray) return;

    current = pList->head.next;
    for (int i = 0; i < numberOfProduct; i++) {
        productArray[i] = (Product*)current->key;
        current = current->next;
    }

    // Sort the array
    qsort(productArray, numberOfProduct, sizeof(Product*), compareByName);

    // Update the linked list with the sorted order
    current = pList->head.next;
    for (int i = 0; i < numberOfProduct; i++) {
        current->key = productArray[i];
        current = current->next;
    }
    pMarket->sortOption = eSortByProductName;
    free(productArray);
}


int compareByName(const void* a, const void* b) {
    const Product* productA = (const Product*)a;
    const Product* productB = (const Product*)b;
    return strcmp(productA->name, productB->name);
}

void sortProductListByQuantity(SuperMarket* pMarket, LIST* pList) {
    if (!pList || !pList->head.next) return;
    
    NODE* current = pList->head.next;
    int numberOfProduct = pMarket->productCount;
    // Create an array of pointers to the products
    Product** productArray = (Product**)malloc(numberOfProduct * sizeof(Product*));
    if (!productArray) return;

    current = pList->head.next;
    for (int i = 0; i < numberOfProduct; i++) {
        productArray[i] = (Product*)current->key;
        current = current->next;
    }

    // Sort the array
    qsort(productArray, numberOfProduct, sizeof(Product*), compareByQuantity);

    // Update the linked list with the sorted order
    current = pList->head.next;
    for (int i = 0; i < numberOfProduct; i++) {
        current->key = productArray[i];
        current = current->next;
    }
    pMarket->sortOption = eSortByProductQuantity;
    free(productArray);
}

int compareByQuantity(const void* a, const void* b) {
    const Product* productA = (const Product*)a;
    const Product* productB = (const Product*)b;
    return productA->count - productB->count;
}

void sortProductListByPrice(SuperMarket* pMarket, LIST* pList)
{
    if (!pList || !pList->head.next) return;

    NODE* current = pList->head.next;
    int numberOfProduct = pMarket->productCount;

    // Create an array of pointers to the products
    Product** productArray = (Product**)malloc(numberOfProduct * sizeof(Product*));
    if (!productArray) return;

    current = pList->head.next;
    for (int i = 0; i < numberOfProduct; i++) {
        productArray[i] = (Product*)current->key;
        current = current->next;
    }

    // Sort the array
    qsort(productArray, numberOfProduct, sizeof(Product*), compareByPrice);

    // Update the linked list with the sorted order
    current = pList->head.next;
    for (int i = 0; i < numberOfProduct; i++) {
        current->key = productArray[i];
        current = current->next;
    }
    pMarket->sortOption = eSortByProducPrice;
    free(productArray);
}

int compareByPrice(const void* a, const void* b) {
    const Product* productA = (const Product*)a;
    const Product* productB = (const Product*)b;
    return (productA->price > productB->price) - (productA->price < productB->price);
}

int searchProduct(SuperMarket* pMarket) {
    if (!pMarket || !pMarket->productList.head.next) {
        printf("Product list is empty or invalid.\n");
        return 0;
    }

    if (pMarket->sortOption == eNoSorted) {
        printf("Product list is not sorted. Search cannot be performed.\n");
        return 0;
    }
    
    printAllProducts(pMarket);
    Product key = getSearchKey(pMarket->sortOption);

    int numberOfProduct = pMarket->productCount;
    if (numberOfProduct == 0) {
        printf("No products available for searching.\n");
        return 0;
    }
    
    Product* productArray = (Product*)malloc(numberOfProduct * sizeof(Product));
    if (!productArray) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    NODE* current = pMarket->productList.head.next;
    for (int i = 0; i < numberOfProduct; i++) {
        Product* product = (Product*)current->key;
        productArray[i] = *product;
        current = current->next;
    }
    
    int (*compareFunc)(const void*, const void*) = getComparisonFunction(pMarket->sortOption);
    if (!compareFunc) {
        printf("Invalid sort option.\n");
        free(productArray);
        return 0;
    }

    
    Product* foundProduct = (Product*)bsearch(
            &key,
            productArray,
            numberOfProduct,
            sizeof(Product),
            compareFunc
        );
    
    if (foundProduct) {
        printf("Product found:\n");
        printProduct(foundProduct);
        free(productArray);
        return 1;
    } else {
        printf("Product not found.\n");
        return 0;
    }
    
    //If we want to return the product that have found
//    Product** productArray = (Product**)malloc(numberOfProduct * sizeof(Product*));
//    if (!productArray) {
//        printf("Memory allocation failed.\n");
//        return NULL;
//    }
//
//    NODE* current = pMarket->productList.head.next;
//    for (int i = 0; i < numberOfProduct; i++) {
//        productArray[i] = (Product*)current->key;
//        current = current->next;
//    }
//
//    Product* foundProduct = (Product*)bsearch(
//        &key,
//        productArray,
//        numberOfProduct,
//        sizeof(Product*),
//        compareByName
//    );
//
//    free(productArray);
//
//    if (foundProduct) {
//        printf("Product found:\n");
//        printProduct(foundProduct);
//        return foundProduct;
//    } else {
//        printf("Product not found.\n");
//        return NULL;
//    }
}

Product getSearchKey(eSortOptions sortOption) {
    Product key;
    memset(&key, 0, sizeof(Product));

    switch (sortOption) {
        case eSortByProductName:
            do {
                printf("enter product name up to %d chars\n", NAME_LENGTH);
                myGets(key.name, sizeof(key.name));
                } while (checkEmptyString(key.name));
            break;

        case eSortByProductQuantity:
            do {
                printf("Enter a positive integer for product quantity:\n");
                if (scanf("%d", &key.count) != 1) {
                    while (getchar() != '\n'); // Clear invalid input
                    key.count = -1;
                }
            } while (key.count < 0);
            break;

                case eSortByProducPrice:
                    do {
                        printf("Enter a positive float for product price:\n");
                        if (scanf("%f", &key.price) != 1) {
                            while (getchar() != '\n');
                            key.price = -1.0;
                        }
                    } while (key.price < 0);
                    break;

        default:
            printf("Invalid sort option.\n");
            break;
    }

    return key;
}

// Function to get the comparison function based on the sortOption
int (*getComparisonFunction(eSortOptions sortOption))(const void*, const void*) {
    switch (sortOption) {
        case eSortByProductName:
            return compareByName;
        case eSortByProductQuantity:
            return compareByQuantity;
        case eSortByProducPrice:
            return compareByPrice;
        default:
            return NULL;
    }
}

int saveSuperMarketData(const SuperMarket* pMarket, const char* productFileName, const char* customerFileName){

    if (!saveProductsToBinary(pMarket, productFileName)) {
        printf("Error saving products to binary file\n");
        return 0;
    }

    if (!saveCustomersToTextFile(pMarket, customerFileName)) {
        printf("Error saving customers to text file\n");
        return 0;
    }

    printf("Supermarket data saved successfully.\n");
    return 1;
}

int saveProductsToBinary(const SuperMarket* pMarket, const char* fileName) {
    FILE* fp = fopen(fileName, "wb");
    if (!fp) {
        perror("Error opening file for writing");
        return 0;
    }

    size_t nameLen = strlen(pMarket->name) + 1;
    fwrite(&nameLen, sizeof(size_t), 1, fp);
    fwrite(pMarket->name, sizeof(char), nameLen, fp);

    fwrite(&pMarket->productCount, sizeof(int), 1, fp);

    NODE* current = pMarket->productList.head.next;
    while (current) {
        Product* pProduct = (Product*)current->key;
        fwrite(pProduct, sizeof(Product), 1, fp);
        current = current->next;
    }
    fclose(fp);
    return 1;
}

int saveCustomersToTextFile(const SuperMarket* pMarket, const char* fileName) {
    FILE* fp = fopen(fileName, "w");
    if (!fp) {
        perror("Error opening file for writing");
        return 0;
    }

    fprintf(fp, "%d\n", pMarket->customerCount);

    NODE* current = pMarket->customerList.head.next;
    while (current) {
        Customer* pCustomer = (Customer*)current->key;

        fprintf(fp, "%s\n", pCustomer->name);

        fprintf(fp, "%s\n", pCustomer->id);

        if (pCustomer->type == CLUB_MEMBER) {

            ClubMember* pMember = (ClubMember*)pCustomer;
            fprintf(fp, "1 %d\n", pMember->totalMonths);
            
        } else {
            
            fprintf(fp, "0 0\n");
        }

        current = current->next;
    }

    fclose(fp);
    return 1;
}

int loadSuperMarketData(SuperMarket* pMarket, const char* productFileName, const char* customerFileName) {
    if (!loadProductsFromBinary(pMarket, productFileName)) {
        printf("Error loading products from binary file\n");
        return 0;
    }

    if (!loadCustomersFromTextFile(pMarket, customerFileName)) {
        printf("Error loading customers from text file\n");
        return 0;
    }

    return 1;
}

int loadCustomersFromTextFile(SuperMarket* pMarket, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        perror("Error opening file for reading");
        return 0;
    }

    int customerCount;
    if (fscanf(fp, "%d\n", &customerCount) != 1) {
        fclose(fp);
        return 0;
    }

    L_init(&pMarket->customerList);
    pMarket->customerCount = 0;

    for (int i = 0; i < customerCount; i++) {
        int isClubMember, totalMonths;

        char name[MAX_STR_LEN];
        if (!fgets(name, MAX_STR_LEN, fp)) {
            fclose(fp);
            return 0;
        }
        name[strcspn(name, "\n")] = '\0';

        char id[CUSTOMER_ID_LENGTH + 1];
        if (fscanf(fp, "%s\n", id) != 1) {
            fclose(fp);
            return 0;
        }

        if (fscanf(fp, "%d %d\n", &isClubMember, &totalMonths) != 2) {
            fclose(fp);
            return 0;
        }

        Customer* pCustomer = NULL;
        if (isClubMember) {
            ClubMember* pMember = (ClubMember*)malloc(sizeof(ClubMember));
            if (!pMember) {
                fclose(fp);
                return 0;
            }
            pMember->base.type = CLUB_MEMBER;
            pMember->totalMonths = totalMonths;
            pMember->base.name = strdup(name);
            pMember->base.id = strdup(id);
            pMember->base.pCart = NULL;
            pCustomer = (Customer*)pMember;
        } else {
            pCustomer = (Customer*)malloc(sizeof(Customer));
            if (!pCustomer) {
                fclose(fp);
                return 0;
            }
            pCustomer->type = REGULAR_CUSTOMER;
            pCustomer->name = strdup(name);
            pCustomer->id = strdup(id);
            pCustomer->pCart = NULL;
        }

        if (!L_insert(&pMarket->customerList.head, (DATA)pCustomer)) {
            free(pCustomer->name);
            free(pCustomer->id);
            free(pCustomer);
            fclose(fp);
            return 0;
        }

        pMarket->customerCount++;
    }

    fclose(fp);
    return 1;
}

int loadProductsFromBinary(SuperMarket* pMarket, const char* fileName) {
    FILE* fp = fopen(fileName, "rb");
    if (!fp) {
        printf("File does not exist. Starting with an empty market.\n");
        return 0;
    }

    size_t nameLen;
    fread(&nameLen, sizeof(size_t), 1, fp);
    pMarket->name = (char*)malloc(nameLen);
    fread(pMarket->name, sizeof(char), nameLen, fp);

    fread(&pMarket->productCount, sizeof(int), 1, fp);

    L_init(&pMarket->productList);

    for (int i = 0; i < pMarket->productCount; i++) {
        Product* pProduct = (Product*)malloc(sizeof(Product));
        fread(pProduct, sizeof(Product), 1, fp);
        L_insert(&(pMarket->productList.head), (DATA)pProduct);
    }
    fclose(fp);
    return 1;
}

//int loadSuperMarketFromBinary(SuperMarket* pMarket, const char* fileName) {
//    FILE* fp = fopen(fileName, "rb");
//    if (!fp) {
//        printf("Error opening file for reading.\n");
//        return 0;
//    }
//
//    // Read supermarket name
//    char name[MAX_STR_LEN];
//    fread(name, sizeof(char), MAX_STR_LEN, fp);
//    pMarket->name = strdup(name);
//
//    // Read the number of products
//    fread(&pMarket->productCount, sizeof(int), 1, fp);
//
//    // Initialize product list
//    L_init(&pMarket->productList);
//    //L_init(&pMarket->customerList);
//
//        //pMarket->productCount
//    // Read each product
//    for (int i = 0; i < 1; i++) {
//        Product* pProduct = (Product*)malloc(sizeof(Product));
//        if (!pProduct) {
//            fclose(fp);
//            return 0;
//        }
//
//        // Read product name
//        fread(pProduct->name, sizeof(char), NAME_LENGTH + 1, fp);
//
//        // Read barcode
//        fread(pProduct->barcode, sizeof(char), BARCODE_LENGTH + 1, fp);
//
//        // Read product type
//        fread(&pProduct->type, sizeof(eProductType), 1, fp);
//
//        // Read price
//        fread(&pProduct->price, sizeof(float), 1, fp);
//
//        // Read count
//        fread(&pProduct->count, sizeof(int), 1, fp);
//
//        // Read expiry date
//        fread(&pProduct->expiryDate, sizeof(Date), 1, fp);
//
//        // Insert the product into the product list
//        if (!L_insert(&(pMarket->productList.head), (DATA)pProduct)) {
//            free(pProduct);
//            fclose(fp);
//            return 0;
//        }
//        printProduct(pProduct);
//    }
//
//    fclose(fp);
//    printf("Supermarket data loaded successfully from %s.\n", fileName);
//    return 1;
//}

void freeMarket(SuperMarket* pMarket)
{
    free(pMarket->name);

    L_free(&pMarket->productList);

    L_free(&pMarket->customerList);
}
