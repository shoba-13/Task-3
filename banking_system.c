#include <stdio.h>

#define FILE_NAME "accounts.dat"

struct Account {
    int accountNumber;
    char name[50];
    double balance;
};

int accountExists(int accountNumber) {
    FILE *fp = fopen(FILE_NAME, "rb");
    struct Account account;

    if (fp == NULL) {
        return 0;
    }

    while (fread(&account, sizeof(struct Account), 1, fp) == 1) {
        if (account.accountNumber == accountNumber) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void createAccount(void) {
    FILE *fp;
    struct Account account;

    printf("\nEnter account number: ");
    if (scanf("%d", &account.accountNumber) != 1) {
        printf("Invalid account number.\n");
        while (getchar() != '\n');
        return;
    }

    if (accountExists(account.accountNumber)) {
        printf("An account with this number already exists.\n");
        return;
    }

    printf("Enter account holder name: ");
    scanf(" %49[^\n]", account.name);

    printf("Enter initial deposit amount: ");
    if (scanf("%lf", &account.balance) != 1 || account.balance < 0) {
        printf("Invalid amount. Initial deposit cannot be negative.\n");
        while (getchar() != '\n');
        return;
    }

    fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        printf("Unable to open the account file.\n");
        return;
    }

    fwrite(&account, sizeof(struct Account), 1, fp);
    fclose(fp);

    printf("Account created successfully!\n");
}

void depositMoney(void) {
    FILE *fp = fopen(FILE_NAME, "rb+");
    struct Account account;
    int accountNumber, found = 0;
    double amount;

    if (fp == NULL) {
        printf("\nNo accounts found. Please create an account first.\n");
        return;
    }

    printf("\nEnter account number: ");
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid account number.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    printf("Enter deposit amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Please enter a valid amount greater than zero.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    while (fread(&account, sizeof(struct Account), 1, fp) == 1) {
        if (account.accountNumber == accountNumber) {
            account.balance += amount;
            fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
            fwrite(&account, sizeof(struct Account), 1, fp);
            found = 1;
            printf("Deposit successful!\n");
            printf("Updated balance: %.2f\n", account.balance);
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

void withdrawMoney(void) {
    FILE *fp = fopen(FILE_NAME, "rb+");
    struct Account account;
    int accountNumber, found = 0;
    double amount;

    if (fp == NULL) {
        printf("\nNo accounts found. Please create an account first.\n");
        return;
    }

    printf("\nEnter account number: ");
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid account number.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    printf("Enter withdrawal amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Please enter a valid amount greater than zero.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    while (fread(&account, sizeof(struct Account), 1, fp) == 1) {
        if (account.accountNumber == accountNumber) {
            found = 1;

            if (amount > account.balance) {
                printf("Insufficient balance. Withdrawal cancelled.\n");
            } else {
                account.balance -= amount;
                fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
                fwrite(&account, sizeof(struct Account), 1, fp);
                printf("Withdrawal successful!\n");
                printf("Remaining balance: %.2f\n", account.balance);
            }
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

void checkBalance(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    struct Account account;
    int accountNumber, found = 0;

    if (fp == NULL) {
        printf("\nNo accounts found. Please create an account first.\n");
        return;
    }

    printf("\nEnter account number: ");
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid account number.\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }

    while (fread(&account, sizeof(struct Account), 1, fp) == 1) {
        if (account.accountNumber == accountNumber) {
            printf("\n--- Account Details ---\n");
            printf("Account Number : %d\n", account.accountNumber);
            printf("Account Holder : %s\n", account.name);
            printf("Current Balance: %.2f\n", account.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

int main(void) {
    int choice;

    do {
        printf("\n========== BANKING SYSTEM ==========\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Balance Enquiry\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                printf("Thank you for using the Banking System!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
