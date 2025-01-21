# Supermarket Management System

## Introduction
The **Supermarket Management System** is a C-based program designed to efficiently manage the operations of a supermarket. It allows you to handle:
- **Products**: Adding, updating, displaying, and sorting products.
- **Customers**: Registering, searching, and managing customers, including special "club members."
- **Shopping Activities**: Managing shopping carts, calculating totals, and handling purchases.
- **Data Management**: Saving and loading supermarket data using files for persistence.

The project uses **linked lists** for dynamic data structures, ensuring scalability and flexibility.

---

## Features
### Product Management
- Add new products with a **unique barcode**.
- Update the stock quantity of existing products.
- Sort products by name, quantity, or price.
- Save and load product data using binary files.

### Customer Management
- Register customers with a **unique ID**.
- Differentiate between **regular customers** and **club members**.
- Search customers by name or ID.
- Save and load customer data in text format.

### Shopping Cart Management
- Add products to a customer's shopping cart.
- Calculate the total price of the cart.
- Manage cart items dynamically (add/remove items).
- Clear the cart after checkout or cancellation.

### Sorting and Searching
- Sort products dynamically based on user-defined criteria (name, quantity, price).
- Search for products using various attributes.

### Persistence
- Save and load supermarket data, including products and customers, to/from files.

---

## File Structure

SupermarketManagement/
├── Cfolder/              # Source code files
│   ├── ClubMember.c      # Club member-specific logic
│   ├── Customer.c        # Customer-related logic
│   ├── Date.c            # Date utilities
│   ├── General.c         # General utility functions
│   ├── Product.c         # Product management logic
│   ├── ShoppingCart.c    # Shopping cart operations
│   ├── ShoppingItem.c    # Shopping item operations
│   ├── Supermarket.c     # Supermarket core functionality
│   └── list.c            # Generic linked list implementation
│
├── Hfolder/              # Header files
│   ├── ClubMember.h      # Club member-specific declarations
│   ├── Customer.h        # Customer-related declarations
│   ├── Date.h            # Date utilities
│   ├── General.h         # General utility declarations
│   ├── Product.h         # Product-related declarations
│   ├── ShoppingCart.h    # Shopping cart declarations
│   ├── ShoppingItem.h    # Shopping item declarations
│   ├── Supermarket.h     # Supermarket core declarations
│   ├── def.h             # General definitions and enums
│   └── list.h            # Linked list declarations
│
├── main.c                # Main entry point of the program
├── main.h                # Main program declarations
├── .gitignore            # Git configuration file
└── README.md             # Project documentation (this file)

---

## Features
### Product Management
- Add new products with **unique barcodes**.
- Update stock quantities for existing products.
- Sort products by name, price, or quantity.
- Display products by type or all products.
- Save and load product data in binary files.

### Customer Management
- Register customers with **unique IDs**.
- Handle **regular customers** and **club members** with discounts.
- Search customers by name or ID.
- Save and load customer data in text files.

### Shopping and Cart Management
- Allow customers to shop and manage carts dynamically.
- Add and remove items from the shopping cart.
- Calculate the total price of the cart.
- Clear the cart after checkout or cancellation.

### Persistence
- Save and load supermarket data (products and customers).
- Use text and binary files for storage.

---

## How to Compile and Run
### Requirements
- **C Compiler**: GCC, Clang, or MinGW.
- Standard C libraries (`stdio.h`, `stdlib.h`, etc.).

### Steps
1. **Navigate to the project directory**:
   ```bash
   cd SupermarketManagement
2. Compile the program::
   ```bash
   cd SupermarketManagementCompile the program:
   
3. Run the executable:
   ```bash
   ./supermarket

---

## Modules Overview:
### Supermarket (Supermarket.h/c)
Core structure: Manages products and customers.
- Key functions:
* addProduct: Add a product.
* addCustomer: Register a new customer.
* doShopping: Handle a customer's shopping session.
### Product (Product.h/c)
Manages product details (name, barcode, price, etc.).
- Key functions:
* initProduct: Initialize a product.
* printProduct: Display product details.
* getProductByBarcode: Find a product by barcode.
### Customer (Customer.h/c)
Handles customer details (name, ID, cart, type).
- Key functions:
*initCustomer: Register a customer.
* FindCustomerById: Find a customer by ID.
* pay: Handle checkout.
### Club Member (ClubMember.h/c)
Extension of customer with club-specific benefits.
- Key functions:
* calculateDiscount: Apply discounts.
### Shopping Cart (ShoppingCart.h/c)
Manages dynamic cart data for customers.
- Key functions:
* addItemToCart: Add an item to the cart.
* getTotalPrice: Calculate the total price of the cart.
### Shopping Item (ShoppingItem.h/c)
Represents items in the cart.
- Key functions:
* createItem: Create a new shopping item.
* printItem: Print item details.
### Linked List (list.h/c)
Implements a generic linked list structure.
* Key functions:
* L_insert: Insert a node.
* L_find: Find a node by key.
* L_free: Free all nodes.
### Utilities (General.h/c, Date.h/c)
General utilities:
* getPositiveInt, checkEmptyString, etc.
* Date utilities:
* initDate, getDateStr.

---

## How to Use:
When running the program, you’ll see a menu to navigate the system
1 - Show Supermarket Details
2 - Add Product
3 - Add Customer
4 - Customer Shopping
5 - View Shopping Cart
6 - Manage Shopping Cart
7 - Print Products by Type
8 - Quit
Simply select an option and follow the prompts.

---

## Future Improvements:
- Add a graphical user interface (GUI).
- Enhance file management with JSON/CSV support.
- Introduce advanced customer analytics.

---

## License:
This project is open-source under the MIT License.

---

## Contributing:
Contributions are welcome! Feel free to fork this repository and submit pull requests.
