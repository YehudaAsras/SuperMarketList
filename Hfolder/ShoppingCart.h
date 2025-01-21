#pragma once
#include "ShoppingItem.h"
#include "def.h"
#include "list.h"

typedef struct {
    LIST head;
} ShoppingCart;


void			initCart(ShoppingCart* pCart);
float			getTotalPrice(const ShoppingCart* pCart);
ShoppingItem*	getItemByBarcode(ShoppingCart* pCart, const char* barcode);
int				addItemToCart(ShoppingCart* pCart, const char* name, float price, int count);
float			printShoppingCart(const ShoppingCart* pCart);

void			freeShoppingCart(ShoppingCart* pCart);
