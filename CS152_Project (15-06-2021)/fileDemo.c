#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct itemInfo
{
    char medName[128];
    // DATE_T medExp;
    int qty;
    char refID[32]; // 8 character ID
    char type[32]; // eg. : tablet, syrup, injection, inhalable, etc.
} ITEM_T;

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

void mainMenu(int ch)
{

A:
    switch (ch)
    {
    case 1:
        //newPurchase();
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
        //changeEncryptionKey();
        break;

    case 6:
        exit(0);
        break;

    default:
        printf("\nInvalid entry. Try again ... ");
        goto A;
        break;
    }
}

void addItem()
{
    FILE* fp;
    ITEM_T newrecord;

    // Open for binary writing
    fp = fopen("data.bin", "wb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    // Just write three records, so we have
    // something to play with. Normally you would
    // do this with a loop and/or user input

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

    fclose(fp);
}

void viewItems()
{
    FILE* fp;
    ITEM_T myrecord;

    fp = fopen("data.bin", "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    printf("The following records are in the binary file\n");
    while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
    {
        printf("%s\n", myrecord.medName);
        printf("%d\n", myrecord.qty);
        printf("%s\n", myrecord.refID);
        printf("%s\n", myrecord.type);
    }
    fclose(fp);
}

void deleteItem()
{
    FILE* fp;
    FILE* fp_tmp;
    int found = 0;
    ITEM_T myrecord;
    char searchname[16] = "asthalin";

    fp = fopen("data.bin", "rb");
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
    remove("data.bin");
    rename("tmp.bin", "data.bin");
}

int main()
{
    mainMenu(2);
    mainMenu(4);
    mainMenu(3);
    mainMenu(4);
    mainMenu(6);
    return 0;
}