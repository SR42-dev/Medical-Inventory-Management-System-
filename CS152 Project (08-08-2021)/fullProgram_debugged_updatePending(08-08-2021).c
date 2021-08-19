// 'press enter to continue' menu feature, menu numbering to be updated

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// create the following binary files beforehand
char path1[] = "data.bin";
char path2[] = "paymentInfo.bin";

// file pointer location
FILE* fpl = 0;

int recCount = 0;

typedef struct date
{
    int dd;
    int mm;
    int yy;

} DATE_T;

typedef struct paymentInfo
{
    char cardNum[16];
    DATE_T expDate;
    int cvv;
    float deduct;

} PAY_T;

typedef struct itemInfo
{
    char medName[128];
    DATE_T medMan;
    DATE_T medExp;
    float unitCost;
    int qty;
    int safetyStock;
    char refID[32]; // 8 character ID
    char type[32]; // eg. : tablet, syrup, injection, inhalable, etc.
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

int dateCompare(DATE_T d1, DATE_T d2) // function to accept date variables and compare - returns 1(d1 > d2), 0(d1 = d2), -1(d1 < d2)
{
    if (d1.yy > d2.yy) return 1;
    else if (d1.yy < d2.yy) return -1;
    else
    {
        if (d1.mm > d2.mm) return 1;
        else if (d1.mm < d2.mm) return -1;
        else
        {
            if (d1.dd > d2.dd) return 1;
            else if (d1.dd < d2.dd) return -1;
            else return 0;
        }
    }
}

void mainMenu(int ch)
{

A:
    switch (ch)
    {
    case 1:
        newPurchase();
        break;

    case 2:
        addItem();
        break;

    case 3:
        deleteItem();
        break;

    case 4:
        viewItems();
        break;

    case 5:
        viewPaymentInfo();
        break;

    case 6:
        reviewWarnings();
        break;

    case 7:
        //changeEncryptionKey();
        break;

    case 8:
        exit(0);
        break;

    default:
        printf("\nInvalid entry. Try again ... ");
        goto A;
        break;
    }
}

void newPurchase()
{
    FILE* fp;
    FILE* fp_tmp;
    FILE* fp_pay;
    int found = 0;
    int purqty;
    float totalCost;
    ITEM_T myrecord;
    PAY_T mypayment;
    char searchname[16];
    int i = 0;

    fp = fopen(path1, "r");
    if (!fp)
    {
        printf("Unable to open file");
        exit(0);
    }
    fp_tmp = fopen("tmp.bin", "wb");
    if (!fp_tmp)
    {
        printf("Unable to open temp file");
        exit(0);
    }
    fp_pay = fopen(path2, "wb");
    if (!fp_pay)
    {
        printf("Unable to open payment information file");
        exit(0);
    }

    do
    {
        recCount++;
        found = 0;
        printf("\n____________________________________________________________________________________\n");
        printf("Input the name of the medicine to be purchased : ");
        scanf(" %[^\n]%*c", searchname);
        printf("Input the quantity to be purchased : ");
        scanf(" %d", &purqty);

        printf("Input card number : ");
        scanf(" %[^\n]%*c", mypayment.cardNum);
        printf("Input date of expiry in dd <enter> mm <enter> yy format\n");
        scanf(" %d", &mypayment.expDate.dd);
        scanf(" %d", &mypayment.expDate.mm);
        scanf(" %d", &mypayment.expDate.yy);
        printf("Input CVV : ");
        scanf(" %d", &mypayment.cvv);

        printf("____________________________________________________________________________________\n");

        while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
        {
            if (strcmp(searchname, myrecord.medName) == 0)
            {
                mypayment.deduct = purqty * myrecord.unitCost;
                myrecord.qty -= purqty;
                fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
                fwrite(&mypayment, sizeof(PAY_T), 1, fp_pay);
                found = 1;
            }
            else
            {
                fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
            }
        }

        if (!found) printf("No record(s) found with the requested name: %s\n\n", searchname);

        char writeAnother;
        int writeAnotherCounter = 0;
        do
        {
            printf("\nWould you like to make another purchase? (y/n) ");
            scanf("%s", &writeAnother);
            switch (writeAnother)
            {
            case 'y':
                i = 0;
                writeAnotherCounter = 1;
                break;

            case 'n':
                i = 1;
                writeAnotherCounter = 1;
                break;

            default:
                printf("Please enter a valid condition! \n");
                break;
            }
        } while (writeAnotherCounter != 1);

    } while (i != 1);

    fclose(fp);
    fclose(fp_tmp);
    fclose(fp_pay);
    remove(path1);
    rename("tmp.bin", path1);
}

void addItem()
{
    FILE* fp;
    ITEM_T newrecord;
    int i = 0;

    // Open for binary writing
    fp = fopen(path1, "wb");

    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    /*
    // test cases (old version)
    strcpy(newrecord.medName, "paracetamol");
    newrecord.qty = 12;
    strcpy(newrecord.refID, "A1B2C3");
    strcpy(newrecord.type, "Tablet");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

    strcpy(newrecord.medName, "benadryl");
    newrecord.qty = 13;
    strcpy(newrecord.refID, "A1B2C4");
    strcpy(newrecord.type, "Syrup");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

    strcpy(newrecord.medName, "asthalin");
    newrecord.qty = 14;
    strcpy(newrecord.refID, "A1B2C5");
    strcpy(newrecord.type, "Inhalable");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

    strcpy(newrecord.medName, "covaxin");
    newrecord.qty = 12;
    strcpy(newrecord.refID, "A1B2C6");
    strcpy(newrecord.type, "Injection");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);
    */
    do
    {
        recCount++;
        printf("\n____________________________________________________________________________________\nInput medicine details : \n____________________________________________________________________________________\n");
        printf("Input name (w/ brand) : ");
        scanf(" %[^\n]%*c", newrecord.medName);
        printf("Input date of manufacture in dd <enter> mm <enter> yy format\n");
        scanf(" %d", &newrecord.medMan.dd);
        scanf(" %d", &newrecord.medMan.mm);
        scanf(" %d", &newrecord.medMan.yy);
        printf("Input date of expiry in dd <enter> mm <enter> yy format\n");
        scanf(" %d", &newrecord.medExp.dd);
        scanf(" %d", &newrecord.medExp.mm);
        scanf(" %d", &newrecord.medExp.yy);
        printf("Input cost per unit : ");
        scanf(" %f", &newrecord.unitCost);
        printf("Input stock quantity : ");
        scanf(" %d", &newrecord.qty);
        printf("Input safety stock quantity : ");
        scanf(" %d", &newrecord.safetyStock);
        printf("Input reference ID : ");
        scanf(" %[^\n]%*c", newrecord.refID);
        printf("Input type (manner of consumption) : ");
        scanf(" %[^\n]%*c", newrecord.type);
        printf("\n____________________________________________________________________________________\n");

        fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

        char writeAnother;
        int writeAnotherCounter = 0;
        do
        {
            printf("\nWould you like to write another record? (y/n) ");
            scanf("%s", &writeAnother);
            switch (writeAnother)
            {
            case 'y':
                i = 0;
                writeAnotherCounter = 1;
                break;

            case 'n':
                i = 1;
                writeAnotherCounter = 1;
                break;

            default:
                printf("Please enter a valid condition! \n");
                break;
            }
        } while (writeAnotherCounter != 1);

    } while (i != 1);
        
    fclose(fp);
}

void viewItems()
{
    FILE* fp;
    ITEM_T myrecord;
    int count = 1;

    fp = fopen(path1, "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    printf("\n____________________________________________________________________________________\nThe following records are in the binary file: \n____________________________________________________________________________________\n");
    while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
    {
        printf("\n");
        printf("Serial number : %d\n", count++);
        printf("Name : %s\n", myrecord.medName);
        printf("Date of manufacture : %d / %d / %d\n", myrecord.medMan.dd, myrecord.medMan.mm, myrecord.medMan.yy);
        printf("Date of expiry : %d / %d / %d\n", myrecord.medExp.dd, myrecord.medExp.mm, myrecord.medExp.yy);
        printf("Cost per unit : %f\n", myrecord.unitCost);
        printf("Quantity in stock : %d\n", myrecord.qty);
        printf("Safety stock : %d\n", myrecord.safetyStock);
        printf("Reference ID : %s\n", myrecord.refID);
        printf("Type : %s\n", myrecord.type);
        printf("\n");
    }

    fclose(fp);
}

void viewPaymentInfo()
{
    FILE* fp;
    PAY_T myrecord;
    int count = 1;

    fp = fopen(path2, "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    printf("\n____________________________________________________________________________________\nThe following records are in the binary file: \n____________________________________________________________________________________\n");
    while (fread(&myrecord, sizeof(PAY_T), 1, fp) != NULL)
    {
        printf("\n");
        printf("Serial number : %d\n", count++);
        printf("Card number : %s\n", myrecord.cardNum);
        printf("Date of expiry : %d / %d / %d\n", myrecord.expDate.dd, myrecord.expDate.mm, myrecord.expDate.yy);
        printf("CVV : %d\n", myrecord.cvv);
        printf("Amount deducted : %f\n", myrecord.deduct);
        printf("\n");
    }

    fclose(fp);
}

void deleteItem()
{
    FILE* fp;
    FILE* fp_tmp;
    int found = 0;
    ITEM_T myrecord;
    char searchname[16];

    printf("\n____________________________________________________________________________________\n");
    printf("Input the name of the record to be deleted : ");
    scanf(" %[^\n]%*c", searchname);
    printf("____________________________________________________________________________________\n");

    fp = fopen(path1, "rb");
    if (!fp)
    {
        printf("Unable to open file");
        exit(0);
    }
    fp_tmp = fopen("tmp.bin", "wb");
    if (!fp_tmp)
    {
        printf("Unable to open temp file");
        exit(0);
    }

    while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
    {
        if (strcmp(searchname, myrecord.medName) == 0)
        {
            printf("A record with requested name found and deleted.\n\n");
            found = 1;
        }
        else
        {
            fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
        }
    }

    if (!found)
    {
        printf("No record(s) found with the requested name: %s\n\n", searchname);
    }

    fclose(fp);
    fclose(fp_tmp);
    remove(path1);
    rename("tmp.bin", path1);
}

void reviewWarnings()
{
    FILE* fp;
    ITEM_T myrecord;
    DATE_T today;
    int count = 1;

    fp = fopen(path1, "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    today.yy = tm.tm_year + 1900;
    today.mm = tm.tm_mon + 1;
    today.dd = tm.tm_mday;

    printf("\n____________________________________________________________________________________\nThe following records are in the binary file: \n____________________________________________________________________________________\n");
    while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
    {
        if (myrecord.qty <= myrecord.safetyStock)
        {
            printf("\n");
            printf("Serial number : %d\n", count++);
            printf("Name : %s\n", myrecord.medName);
            printf("Date of manufacture : %d / %d / %d\n", myrecord.medMan.dd, myrecord.medMan.mm, myrecord.medMan.yy);
            printf("Date of expiry : %d / %d / %d\n", myrecord.medExp.dd, myrecord.medExp.mm, myrecord.medExp.yy);
            printf("Cost per unit : %f\n", myrecord.unitCost);
            printf("Quantity in stock : %d\n", myrecord.qty);
            printf("Safety stock : %d\n", myrecord.safetyStock);
            printf("Reference ID : %s\n", myrecord.refID);
            printf("Type : %s\n", myrecord.type);
            printf("Warning : Inadequate safety stock. Restock immediately\n", myrecord.type);
            printf("\n");
        }


        if (dateCompare(today, myrecord.medExp) > 0)
        {
            printf("\n");
            printf("Serial number : %d\n", count++);
            printf("Name : %s\n", myrecord.medName);
            printf("Date of manufacture : %d / %d / %d\n", myrecord.medMan.dd, myrecord.medMan.mm, myrecord.medMan.yy);
            printf("Date of expiry : %d / %d / %d\n", myrecord.medExp.dd, myrecord.medExp.mm, myrecord.medExp.yy);
            printf("Cost per unit : %f\n", myrecord.unitCost);
            printf("Quantity in stock : %d\n", myrecord.qty);
            printf("Safety stock : %d\n", myrecord.safetyStock);
            printf("Reference ID : %s\n", myrecord.refID);
            printf("Type : %s\n", myrecord.type);
            printf("Warning : Stock expired. Discard immediately\n", myrecord.type);
            printf("\n");
        }
    }

    fclose(fp);
}

int main()
{
    int ch;
    char dummy;

A:

    delay(4);
    clrscr();

    printf("\n________________________________________________________________________________________\n");
    printf("MENU");
    printf("\n________________________________________________________________________________________\n");
    printf("1. New purchase\n");
    printf("2. Add items\n");
    printf("3. Delete item\n");
    printf("4. View items\n");
    printf("5. View payment information\n");
    printf("6. Review warnings\n");
    printf("7. Change encryption key\n"); // function pending
    printf("8. Exit");
    printf("\n________________________________________________________________________________________\n");
    printf("Input your choice : ");
    scanf(" %d", &ch);
    printf("\n________________________________________________________________________________________\n");

    switch (ch)
    {
    case 1:

        mainMenu(1);
        break;

    case 2:

        mainMenu(2);
        break;

    case 3:

        mainMenu(3);
        break;

    case 4:

        mainMenu(4);
        delay(10);
        break;

    case 5:

        mainMenu(5);
        break;

    case 6:

        mainMenu(6);
        break;
         
    case 7:

        mainMenu(7);
        break;

    case 8:

        mainMenu(8);
        break;

    default:

        printf("\nInvalid entry. Try again ... ");
        goto A;
        break;
    }

    // system("pause");
    goto A;

    return 0;
}