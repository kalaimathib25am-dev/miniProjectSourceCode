#include <stdio.h>
#include <stdlib.h>

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

int main(int argc, char *argv[]) {
    FILE *cfPtr;
    unsigned int choice;

    // 🔹 Change 1: file name from command line
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    if ((cfPtr = fopen(argv[1], "rb+")) == NULL) {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 5) {
        switch (choice) {
            case 1:
                textFile(cfPtr);
                break;
            case 2:
                updateRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// 🔹 Convert binary file to text file
void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        printf("File cannot be created\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    // 🔹 Change 2: correct loop
    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0) {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum, client.lastName,
                    client.firstName, client.balance);
        }
    }

    fclose(writePtr);
}

// 🔹 Update record
void updateRecord(FILE *fPtr) {
    struct clientData client;
    unsigned int account;
    double transaction;

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    // 🔹 validation
    if (account < 1 || account > 100) {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account not found\n");
    } else {
        printf("Current Balance: %.2f\n", client.balance);

        printf("Enter amount (+ deposit / - withdraw): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Updated Balance: %.2f\n", client.balance);
    }
}

// 🔹 Add new record
void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100) {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account already exists\n");
    } else {
        printf("Enter lastname firstname balance: ");
        scanf("%14s %9s %lf",
              client.lastName, client.firstName, &client.balance);

        client.acctNum = account;

        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Record added successfully\n");
    }
}

// 🔹 Delete record
void deleteRecord(FILE *fPtr) {
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100) {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account not found\n");
    } else {
        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);

        printf("Record deleted\n");
    }
}

// 🔹 Menu
unsigned int enterChoice(void) {
    unsigned int choice;

    printf("\n1 - Display accounts\n");
    printf("2 - Update account\n");
    printf("3 - Add account\n");
    printf("4 - Delete account\n");
    printf("5 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}