#pragma once
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#include "list.h"
#include "ShoppingItem.h"

#define EXIT -1

typedef enum
{
    eNoSorted, eSortByProductName, eSortByProductQuantity, eSortByProducPrice, eNofOption
} eSortOptions;

static const char* menuString[eNofOption] = { "No Sorted", "Sort By Product Name",
                                "Sort By Product Quantity", "Sort By Produc Price"};

typedef struct
{
    char* name;        // Supermarket name
    LIST customerList; // List of customers
    LIST productList;  // List of products
    eSortOptions sortOption;
    int customerCount; // Number of customers in the supermarket
    int productCount;  // Number of products in the supermarket
} SuperMarket;

int         initSuperMarket(SuperMarket* pMarket);
void		printSuperMarket(const SuperMarket* pMarket);
int			addProduct(SuperMarket* pMarket);
int			addNewProduct(SuperMarket* pMarket);
int			addCustomer(SuperMarket* pMarket);
int			isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int			doShopping(SuperMarket* pMarket);
Customer*	doPrintCart(SuperMarket* pMarket);
int			manageShoppingCart(SuperMarket* pMarket);
Customer*	getCustomerShopPay(SuperMarket* pMarket);
void		fillCart(SuperMarket* pMarket, ShoppingCart* pCart);
void		clearCart(SuperMarket* pMarket, Customer* pCustomer);

void		printProductByType(SuperMarket* pMarket);
void		printAllProducts(const SuperMarket* pMarket);
void		printAllCustomers(const SuperMarket* pMarket);
int			isBarcodeUnique(const SuperMarket* pMarket, const char* barcode);
int 		isCustomerIdUnique(const SuperMarket* pMarket, const char* id);


int			getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductFromUser(SuperMarket* pMarket, char* barcode);
void		getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer*	FindCustomerByName(SuperMarket* pMarket, const char* name);
Customer*	FindCustomerById(SuperMarket* pMarket, const char* id);

Customer*	getCustomerWhoShop(SuperMarket* pMarket);
Product*	getProductAndCount(SuperMarket* pMarket, int* pCount);

void        sortSoperMarket(SuperMarket* pMarket);
int         showSortMenu();
void        sortProductListByName(SuperMarket* pMarket, LIST* pList);
void        sortProductListByQuantity(SuperMarket* pMarket, LIST* pList);
void        sortProductListByPrice(SuperMarket* pMarket, LIST* pList);
int         compareByName(const void* a, const void* b);
int         compareByQuantity(const void* a, const void* b);
int         compareByPrice(const void* a, const void* b);

int         searchProduct(SuperMarket* market);
Product     getSearchKey(eSortOptions sortOption);
int         (*getComparisonFunction(eSortOptions sortOption))(const void*, const void*);

int         saveSuperMarketData(const SuperMarket* pMarket, const char* productFileName, const char* customerFileName);
int         saveProductsToBinary(const SuperMarket* pMarket, const char* fileName);
int         saveCustomersToTextFile(const SuperMarket* pMarket, const char* fileName);
int         loadSuperMarketData(SuperMarket* pMarket, const char* productFileName, const char* customerFileName);
int         loadProductsFromBinary(SuperMarket* pMarket, const char* fileName);
int         loadCustomersFromTextFile(SuperMarket* pMarket, const char* fileName);

void		freeMarket(SuperMarket* pMarket);

