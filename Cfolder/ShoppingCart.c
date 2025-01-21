#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Hfolder/ShoppingCart.h"
#include "../Hfolder/General.h"
#include "../Hfolder/ShoppingItem.h"

void initCart(ShoppingCart* pCart) {
    if (!pCart) return;

    L_init(&pCart->head);
}

// Calculate the total price of items in the cart
float getTotalPrice(const ShoppingCart* pCart) {
    if (!pCart) return 0.0;

    float price = 0.0;
    NODE* current = pCart->head.head.next;
    
    while (current) {
        ShoppingItem* pItem = (ShoppingItem*)current->key;
        price += (pItem->price * pItem->count);
        current = current->next;
    }

    return price;
}


// Add an item to the shopping cart (with sorting by barcode)
int addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count) {
    if (!pCart || !barcode || price < 0 || count <= 0) return 0;

    ShoppingItem* pItem = getItemByBarcode(pCart, barcode);
    if (!pItem) { // New item    
        pItem = createItem(barcode, price, count);
        if (!pItem) return 0;

        NODE* newNode = (NODE*)malloc(sizeof(NODE));
        if (!newNode) {
            free(pItem);
            return 0;
        }

        newNode->key = pItem;
        
        NODE* current = &(pCart->head.head);
        while (current->next && strcmp(((ShoppingItem*)current->next->key)->barcode, barcode) < 0) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;

    } else {
        pItem->count += count;
    }

    return 1;
}

// Print the shopping cart and return the total price
float printShoppingCart(const ShoppingCart* pCart) {
    float totalPrice = 0.0;
    NODE* current = pCart->head.head.next;

    printf("\nShopping Cart:\n");
    while (current) {
        ShoppingItem* pItem = (ShoppingItem*)current->key;
        printItem(pItem);
        totalPrice += (pItem->price * pItem->count);
        current = current->next;
    }

    printf("Total bill to pay: %.2f\n", totalPrice);
    return totalPrice;
}

// Find an item in the cart by barcode
ShoppingItem* getItemByBarcode(ShoppingCart* pCart, const char* barcode) {
    NODE* current = pCart->head.head.next;

    while (current) {
        ShoppingItem* pItem = (ShoppingItem*)current->key;
        if (strcmp(pItem->barcode, barcode) == 0) {
            return pItem;
        }
        current = current->next;
    }

    return NULL;
}

// Free the memory used by the shopping cart
void freeShoppingCart(ShoppingCart* pCart)
{
    if (!pCart) return;

    NODE* current = pCart->head.head.next;
    NODE* nextNode;

    while (current)
    {
        nextNode = current->next;

        // Free the ShoppingItem
        ShoppingItem* pItem = (ShoppingItem*)current->key;
        free(pItem);
        free(current);

        current = nextNode;
    }

    pCart->head.head.next = NULL;
}
