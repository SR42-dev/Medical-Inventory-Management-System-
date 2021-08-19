// A program to process column values from a .csv spreadsheet (prototype for project)

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
        // addItem();
        addItemCSV();
        break;

    case 3:
        // deleteItem();
        deleteItemCSV();
        break;

    case 4:
        // viewItems();
        viewItemsCSV();
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

    fp = fopen("data.bin", "wb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

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

void addItemCSV()
{
    FILE* fp = fopen("data.csv", "a+");
    ITEM_T newrecord;

    if (!fp) {
        printf("Can't open file\n");
        return 0;
    }

    //data input (manual)
    strcpy(newrecord.medName, "paracetamol");
    newrecord.qty = 12;
    strcpy(newrecord.refID, "A1B2C3");
    strcpy(newrecord.type, "Tablet");
    fprintf(fp, "%s, %d, %s, %s\n", newrecord.medName, newrecord.qty, newrecord.refID, newrecord.type);

    strcpy(newrecord.medName, "benadryl");
    newrecord.qty = 13;
    strcpy(newrecord.refID, "A1B2C4");
    strcpy(newrecord.type, "Syrup");
    fprintf(fp, "%s, %d, %s, %s\n", newrecord.medName, newrecord.qty, newrecord.refID, newrecord.type);

    strcpy(newrecord.medName, "asthalin");
    newrecord.qty = 14;
    strcpy(newrecord.refID, "A1B2C5");
    strcpy(newrecord.type, "Inhalable");
    fprintf(fp, "%s, %d, %s, %s\n", newrecord.medName, newrecord.qty, newrecord.refID, newrecord.type);

    strcpy(newrecord.medName, "covaxin");
    newrecord.qty = 12;
    strcpy(newrecord.refID, "A1B2C6");
    strcpy(newrecord.type, "Injection");
    fprintf(fp, "%s, %d, %s, %s\n", newrecord.medName, newrecord.qty, newrecord.refID, newrecord.type);

    printf("\nNew Entries added to record");

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

void viewItemsCSV()
{
    FILE* fp = fopen("data.csv", "r");

    if (!fp) printf("Can't open file\n");

    else 
    {
  
        char buffer[1024];

        int row = 0;
        int column = 0;

        while (fgets(buffer,1024, fp)) 
        {
            column = 0;
            row++;

            char* value = strtok(buffer, ", ");

            while (value) 
            {

                if (column == 0) printf("Medicine name : ");
            
                if (column == 1) printf("\tQuantity : ");

                if (column == 2) printf("\tReference ID : ");
             
                if (column == 3) printf("\tType :");
     
                printf("%s", value);
                value = strtok(NULL, ", ");
                column++;
            }

            printf("\n");
        }

        fclose(fp);
    }
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

void deleteItemCSV()
{
    FILE* fp;
    FILE* fp_tmp;
    int found = 0;
    ITEM_T myrecord;
    char searchname[16] = "asthalin";

    char buffer[1024];
    int row = 0;
    int column = 0;

    fp = fopen("data.csv", "r");
    if (!fp)
    {
        printf("Unable to open file");
        exit(0);
    }
    fp_tmp = fopen("tmp.csv", "a+");
    if (!fp_tmp)
    {
        printf("Unable to open temp file");
        exit(0);
    }

    while (fgets(buffer, 1024, fp))
    {
        column = 0;
        row++;

        if (row == 1)
            continue;

        char* value = strtok(buffer, ", ");

        while (value)
        {
            if (strcmp(searchname, value) == 0)
            {
                value = strtok(NULL, ", ");
                column++;
                continue;
            }

            if (column == 0) fprintf(fp_tmp, "%s", value);

            if (column == 1) fprintf(fp_tmp, "%s", value);

            if (column == 2) fprintf(fp_tmp, "%s", value);

            if (column == 3) fprintf(fp_tmp, "%s", value);

            
            value = strtok(NULL, ", ");
            column++;
        }
        fprintf(fp_tmp, "\n");
    }

    if (!found)
    {
        printf("No record(s) found with the requested name: %s\n\n", searchname);
    }

    fclose(fp);
    fclose(fp_tmp);
    remove("data.csv");
    rename("tmp.csv", "data.csv");
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