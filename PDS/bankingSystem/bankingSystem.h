#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>

#define MAX_ACCOUNTS 500

typedef struct customerData{
	int accountNumber;
	char name[21];
	char surname[21];
	char address[50];
	char pesel[12];
	float balance;
} customerAccount;

int displayMenuAndChooseAction();
int displaySearchMenuAndChooseAction();
float properWithdrawAmount(customerAccount customer);
float properDepositAmount();
int properChoiceInput(int numberOfOptions);
void properPeselInput(char* pesel);
void propernameInput(char* name);
void properSurnameInput(char* surname);
void properAddressInput(char* address);
void printCustomerAccount(customerAccount customer);
void printTheDatabase();
void resetDatabase();
void createNewAccount();
void makeDeposit();
void makeWithdrawal();
void makeTransfer();
void searchAccount();
void searchByAccountNumber();
void searchByName();
void searchBySurname();
void searchByAddress();
void searchByPesel();
void fileOpenCheck(FILE* fptr);
void fileCloseCheck(FILE* fptr);
void fseekCheck(FILE* fptr, int accountNumber);
void freadCheck(FILE* fptr, customerAccount* customer);
void fwriteCheck(FILE* fptr, customerAccount customer);
bool confirmAction();
bool accountExistenceCheck(customerAccount customer);
bool isAccountEmpty(customerAccount customer);

int displayMenuAndChooseAction() {
	printf("\n\n****************MENU*****************\n");
	printf("_____________________________________\n");
	printf("|                                    |\n");
	printf("| 1. View all accounts in the system |\n");
	printf("| 2. Create a new customer account   |\n");
	printf("| 3. Search for an account           |\n");
	printf("| 4. Make a transfer                 |\n");
	printf("| 5. Make a deposit                  |\n");
	printf("| 6. Make a withdrawal               |\n");
	printf("| 7. Exit the system                 |\n");
	printf("|____________________________________|\n");
	int choice = properChoiceInput(7);
	return choice;
}

int displaySearchMenuAndChooseAction() {
	printf("\n\n***CHOOSE SEARCHING OPTION****\n");
	printf("______________________________\n");
	printf("|                             |\n");
	printf("| 1. Search by account number |\n");
	printf("| 2. Search by name           |\n");
	printf("| 3. Search by surname        |\n");
	printf("| 4. Search by address        |\n");
	printf("| 5. Search by ID             |\n");
	printf("| 6. Go back to menu          |\n");
	printf("|_____________________________|\n");
	int choice = properChoiceInput(6);
	return choice;
}

float properWithdrawAmount(customerAccount customer) {
	float b;
	printf("Insert the amount you would like to withdraw: ");
	int correct = scanf("%f", &b);
	while (!correct || b <= 0 || b > customer.balance || b > FLT_MAX) {
		while (getchar() != '\n');
		printf("Amount of money has to be a positive number smaller than your balance: %.2f!\n", customer.balance);
		printf("Insert the amount you want to withdraw again: ");
		correct = scanf("%f", &b);
	}
	return b;
}

float properDepositAmount() {
	float b;
	printf("Insert the amount you would like to deposit: ");
	int correct = scanf("%f", &b);
	while (!correct || b <= 0 || b > FLT_MAX) {
		while (getchar() != '\n');
		printf("Amount of money has to be a positive number\n");
		printf("Insert the amount you want to deposit again: ");
		correct = scanf("%f", &b);
	}
	return b;
}

int properAccountInput(){
	int accountNumber;
	int correct = scanf("%d", &accountNumber);
	while (!correct || accountNumber <= 0 || accountNumber > MAX_ACCOUNTS) {
		while (getchar() != '\n');
		printf("There can only be 500 accounts in the database!\n");
		printf("Choose the account again: ");
		correct = scanf("%d", &accountNumber);
	}
	return accountNumber;
}

int properChoiceInput(int numberOfOptions) {
	int choice;
	printf("Choose your option: ");
	int correct = scanf("%d", &choice);
	while (!correct || choice <= 0 || choice > numberOfOptions) {
		while (getchar() != '\n');
		printf("Incorrect choice!\n");
		printf("Choose your oprion again: ");
		correct = scanf("%d", &choice);
	}
	return choice;
}

void properPeselInput(char* pesel) {
	while (getchar() != '\n');
	printf("Insert PESEL (11 digits): ");
	int correct = scanf("%11s", pesel);
	while (!correct || strlen(pesel) != 11) {
		while (getchar() != '\n');
		printf("Incorrect input!\n");
		printf("Insert PESEL again: ");
		scanf("%s", pesel);
	}
}

void properNameInput(char* name) {
	while (getchar() != '\n');
	printf("Insert name: ");
	int correct = scanf("%20s", name);
	while (!correct || strlen(name) >= 20) {
		while (getchar() != '\n');
		printf("Incorrect input!\n");
		printf("Insert name again: ");
		scanf("%s", name);
	}
}

void properSurnameInput(char* surname) {
	while (getchar() != '\n');
	printf("Insert surname: ");
	int correct = scanf("%20s", surname);
	while (!correct || strlen(surname) >= 20) {
		while (getchar() != '\n');
		printf("Incorrect input!\n");
		printf("Insert surname again: ");
		scanf("%s", surname);
	}
}

void properAddressInput(char* address) {
	while (getchar() != '\n');
	printf("Insert address: ");
	int correct = scanf("%[^\n]49s", address);
	while (!correct || strlen(address) >= 49) {
		while (getchar() != '\n');
		printf("Incorrect input!\n");
		printf("Insert address again: ");
		scanf("%s", address);
	}
}

void printCustomerAccount(customerAccount customer) {
	printf("*******************************\n");
    printf("*      ACCOUNT NUMBER %04d     *\n", customer.accountNumber);
    printf("********************************\n");
    printf("  NAME: %s\n", customer.name);
    printf("  SURNAME: %s\n", customer.surname);
    printf("  ADDRESS: %s\n", customer.address);
    printf("  PESEL: %s\n", customer.pesel);
    printf("  BALANCE: %.2f\n", customer.balance);
}

void printTheDatabase() {
	customerAccount customer;
    FILE *database;
    database = fopen("clients.dat", "rb");
    fileOpenCheck(database);
    printf("********************************\n");
    printf("*      CUSTOMERS ACCOUNTS      *\n");
    printf("********************************\n");
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        freadCheck(database, &customer);
        if(customer.name[0]) {
            printCustomerAccount(customer);
        }
    }
    fileCloseCheck(database);
}

void resetDatabase() {
    FILE *database;
    database = fopen("clients.dat", "wb");
    fileOpenCheck(database);
    customerAccount customer = {0, "", "", "", "", 0.0};
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        customer.accountNumber = accountNumber;
        fwriteCheck(database, customer);
    }
    fileCloseCheck(database);
}

void createNewAccount() {
	customerAccount newAccount;
    FILE *database;
    customerAccount existingAccount;
    printf("\n********************************\n");
    printf("*      CREATE NEW ACCOUNT      *\n");
    printf("********************************\n");
    properNameInput(newAccount.name);
	properSurnameInput(newAccount.surname);
    properAddressInput(newAccount.address);
    properPeselInput(newAccount.pesel);
    newAccount.balance = 0;
    database = fopen("clients.dat", "rb+");
    fileOpenCheck(database);
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        freadCheck(database, &existingAccount);
        if (!existingAccount.name[0]) {
			if(confirmAction()){
				newAccount.accountNumber = accountNumber;
				fseekCheck(database, accountNumber);
				fwriteCheck(database, newAccount);
				printf("Action successful, account number is: %04d\n", accountNumber);
			}
			break;
        }
        if((accountNumber == MAX_ACCOUNTS) && (existingAccount.name[0])) {
            printf("Database is full, extend it's size.");
        }
    }
    fileCloseCheck(database);
}

void makeDeposit(){
	printf("\n********************************\n");
    printf("*         MAKE A DEPOSIT       *\n");
    printf("********************************\n");
    customerAccount existingAccount;
    FILE *database;
    database = fopen("clients.dat", "rb+");
    fileOpenCheck(database);
    int depositAccount;
    float depositAmount;
    printf("Insert account number to which you would like to deposit: ");
    depositAccount = properAccountInput();
	for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
		freadCheck(database, &existingAccount);
		if (existingAccount.accountNumber == depositAccount) {
			if(!accountExistenceCheck(existingAccount)){
				break;
			}
			fseekCheck(database, accountNumber);
			depositAmount = properDepositAmount();
			if(confirmAction()){
				existingAccount.balance += depositAmount;
				fwriteCheck(database, existingAccount);
				printf("Operation successful, new balance for account number %04d is: %.2f\n", existingAccount.accountNumber, existingAccount.balance);
			}
			break;
		}
	}
    fileCloseCheck(database);
}

void makeWithdrawal(){
	printf("\n********************************\n");
    printf("*       MAKE A WITHDRAWAL      *\n");
    printf("********************************\n");
    customerAccount existingAccount;
    FILE *database;
    database = fopen("clients.dat", "rb+");
    fileOpenCheck(database);
    int withdrawAccount;
    float withdrawAmount;
    printf("Insert account number from which you would like to withdraw: ");
    withdrawAccount = properAccountInput();
	for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
		freadCheck(database, &existingAccount);
		if (existingAccount.accountNumber == withdrawAccount) {
			if(!accountExistenceCheck(existingAccount)){
				break;
			}
			fseekCheck(database, accountNumber);
			if(isAccountEmpty(existingAccount)){
				break;
			}
			withdrawAmount = properWithdrawAmount(existingAccount);
			if(confirmAction()){
				existingAccount.balance -= withdrawAmount;
				fwriteCheck(database, existingAccount);
				printf("Operation successful, new balance for account number %04d is: %.2f\n", existingAccount.accountNumber, existingAccount.balance);
			}
			break;
		}
	}
	fileCloseCheck(database);
}

void makeTransfer(){
	printf("\n********************************\n");
    printf("*        MAKE A TRANSFER       *\n");
    printf("********************************\n");
    customerAccount fromAccount;
    customerAccount toAccount;
    customerAccount auxAccount;
    FILE *database;
    database = fopen("clients.dat", "rb+");
    fileOpenCheck(database);
    int withdrawAccount;
    int depositAccount;
    float amount;
    bool bothExist = true;
    printf("Insert account number from which you would like to transfer: ");
    withdrawAccount = properAccountInput();
    printf("Insert account number to which you would like to transfer: ");
    depositAccount = properAccountInput();
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        freadCheck(database, &auxAccount);
        if (auxAccount.accountNumber == withdrawAccount) {
			if(!accountExistenceCheck(auxAccount)){
				bothExist = false;
				break;
			}
            fseekCheck(database, accountNumber);
			fromAccount = auxAccount;
		}
		if (auxAccount.accountNumber == depositAccount) {
			if(!accountExistenceCheck(auxAccount)){
				bothExist = false;
				break;
			}
            fseekCheck(database, accountNumber);
            toAccount = auxAccount;
		}
	}
	if(!isAccountEmpty(fromAccount) && bothExist){
		amount = properWithdrawAmount(fromAccount);
		if(confirmAction()){
			fromAccount.balance -= amount;
			toAccount.balance += amount;
			fseekCheck(database, toAccount.accountNumber);
			fwriteCheck(database, toAccount);
			fseekCheck(database, fromAccount.accountNumber);
			fwriteCheck(database, fromAccount);
			printf("Operation successful!\nNew balance for account number %04d is: %.2f\nNew balance for account number %04d is: %.2f\n", fromAccount.accountNumber, fromAccount.balance, toAccount.accountNumber, toAccount.balance);
		}
    }
    fileCloseCheck(database);
}

void searchAccount(){
	printf("\n********************************\n");
    printf("*   SEARCHING FOR AN ACCOUNT   *\n");
    printf("********************************\n");
    int choice = displaySearchMenuAndChooseAction();
    switch(choice){
		case 1:
			searchByAccountNumber();
			break;
		case 2:
			searchByName();
			break;
		case 3:
			searchBySurname();
			break;
		case 4:
			searchByAddress();
			break;
		case 5:
			searchByPesel();
			break;
		case 6:
			break;
	}
}

void searchByAccountNumber(){
	customerAccount existingAccount;
    FILE *database;
    database = fopen("clients.dat", "rb");
    fileOpenCheck(database);
    int searchedAccount;
    printf("Insert account number you are looking for: ");
    scanf("%d", &searchedAccount);
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        freadCheck(database, &existingAccount);
        if (existingAccount.accountNumber == searchedAccount) {
			if(!accountExistenceCheck(existingAccount)){
				break;
			}
            printCustomerAccount(existingAccount);
            break;
        }
    }
    fileCloseCheck(database);
}

void searchByName(){
	customerAccount existingAccount;
    FILE *database;
    database = fopen("clients.dat", "rb");
    fileOpenCheck(database);
    char name[21];
    properNameInput(name);
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        freadCheck(database, &existingAccount);
        if (strcmp(existingAccount.name, name) == 0) {
            printCustomerAccount(existingAccount);
        }
    }
    fileCloseCheck(database);
}

void searchBySurname(){
	customerAccount existingAccount;
    FILE *database;
    database = fopen("clients.dat", "rb");
    fileOpenCheck(database);
    char surname[21];
    properSurnameInput(surname);
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        freadCheck(database, &existingAccount);
        if (strcmp(existingAccount.surname, surname) == 0) {
            printCustomerAccount(existingAccount);
        }
    }
    fileCloseCheck(database);
}

void searchByAddress(){
	customerAccount existingAccount;
    FILE *database;
    database = fopen("clients.dat", "rb");
    fileOpenCheck(database);
    char address[50];
    properAddressInput(address);
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
		freadCheck(database, &existingAccount);
        if (strcmp(existingAccount.address, address) == 0) {
            printCustomerAccount(existingAccount);
        }
    }
    fileCloseCheck(database);
}

void searchByPesel(){
	customerAccount existingAccount;
    FILE *database;
    database = fopen("clients.dat", "rb");
    fileOpenCheck(database);
    char pesel[12];
    properPeselInput(pesel);
    for(int accountNumber = 1; accountNumber <= MAX_ACCOUNTS; accountNumber++) {
        freadCheck(database, &existingAccount);
        if (strcmp(existingAccount.pesel, pesel) == 0) {
			printCustomerAccount(existingAccount);
        }
    }
    fileCloseCheck(database);
}

void fileOpenCheck(FILE* fptr){
    if(!fptr) {
        perror("Error during opening clients.dat");
        exit(1);
    }
}

void fileCloseCheck(FILE* fptr){
    if (fclose(fptr) != 0) {
		perror("Error closing clients.dat");
		exit(3);
	}
}

void fseekCheck(FILE* fptr, int accountNumber){
	if(fseek(fptr, (accountNumber - 1) * sizeof(customerAccount), SEEK_SET)) {
		perror("Error seeking in clients.dat");
        exit(4);
    }	
}

void freadCheck(FILE* fptr, customerAccount* customer){
	if(fread(customer, sizeof(customerAccount), 1, fptr) != 1) {
		perror("Error while reading from clients.dat");
        exit(3);
    }
}

void fwriteCheck(FILE* fptr, customerAccount customer){
	if(fwrite(&customer, sizeof(customerAccount), 1, fptr) != 1) {
		perror("Error while updating clients.dat");
        exit(3);
    }
}

bool confirmAction(){
	while(getchar() != '\n');
	printf("Are you sure you want to perform this operation? [y/n]: ");
	char confirm;
	scanf("%c", &confirm);
    while ((toupper(confirm) != 'Y') && (toupper(confirm) != 'N')){
		printf("Incorrect input, try again [y/n]: ");
		while(getchar() != '\n');
		scanf("%c", &confirm);
	}
    if(toupper(confirm) == 'Y'){
		return true;
	}else{
		return false;
	}
}

bool accountExistenceCheck(customerAccount customer){
	if(!customer.name[0]){
		printf("Account with such an account number doesn't exist!\n");
		return false;
	}else{
		return true;
	}
}

bool isAccountEmpty(customerAccount customer){
	if(customer.balance == 0){
		printf("Customer doesn't have any money on their account!\n");
		return true;
	}else{
		return false;
	}
}
