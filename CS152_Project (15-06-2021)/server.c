/*

Description in points:

-	Menu driven system : new purchase, add item, delete items, view database, change encryption key, exit
-	Unique datatypes to hold payment info, items in inventory & bills
-	Encryption method : input -> encryption -> updating binary file for data upload. Decryption and display for data download

Main menu :
	
- New purchase
- Add item
- Delete item
- View item 
- Update item
- View database
- Change encryption key
- Exit

*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "server.h"

typedef struct date 
{
	int day;
	int month;
	int year;

} DATE_T;
	
typedef struct paymentInfo
{
	char cardNum[16];
	DATE_T expDate;
	int cvv;

} PAY_T;

typedef struct itemInfo
{
    char medName[128];
    // DATE_T medExp;
	int qty;
	char refID[16]; // 8 character ID
	char type[16]; // eg. : tablet, syrup, injection, inhalable, etc.

} ITEM_T;

void clrscr()
{
    system("@cls||clear");
}

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

void mainMenu(int ch)
{

	A:
	clrscr();
	switch (ch)
	{
		case 1 :
			newPurchase();
			break;

		case 2 :
			addItem();
			break;

		case 3 :
			deleteItem();
			break;

		case 4 :
			viewItem(); 
			break;

		case 5 :
			changeEncryptionKey();
			break;

		case 6 :
			exit(0);
			break;

		default:
			printf("\nInvalid entry. Try again ... ");
			delay(3);
			goto A;
			break;
	}
}

