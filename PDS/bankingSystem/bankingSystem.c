#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bankingSystem.h"

int main(int argc, char* argv[]) {
	if(argc > 1) {
        if (strcmp(argv[1], "reset") == 0) {
            resetDatabase();
        }
    }
    bool loop = true;
    int choice;
    while(loop){
		choice = displayMenuAndChooseAction();
		switch(choice){
			case 1:
				printTheDatabase();
				break;
			case 2:
				createNewAccount();
				break;
			case 3:
				searchAccount();
				break;
			case 4:
				makeTransfer();
				break;
			case 5:
				makeDeposit();
				break;
			case 6:
				makeWithdrawal();
				break;
			case 7:
				loop = false;
				break;
		}
	}
	return 0;
}
