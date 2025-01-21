#pragma once

#define EXIT -1

typedef enum
{
	eShowSuperMarket, eAddProduct, eAddCustomer, eCustomerDoShopping, ePrintCart, eCustomerManageShoppingCart,
	ePrintProductByType, eSortSoperMarket,eSearchProduct, eNofOptions
} eMenuOptions;

const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping","Print Shopping Cart","Customer Shopping Cart Managment",
								"Print Product By Type", "Sort Sopermarket", "Search Product" };

int menu();
