/*
 * File Name: continue.c
 * Author: Arpit Patel
 * Date: 2025-04-29
 * Description: This file contains the implementation of a function that prompts the user to decide whether to continue or not.
 */


#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>


/*
 * Function: _continue
 * ----------------------------
 * Prompts the user to decide whether to continue or not.
 *
 * @return true if the user chooses 'Y', false if the user chooses 'n'.
 *
 * Description:
 *   This function repeatedly asks the user if they want to continue until a valid response ('Y' or 'n') is received.
 *   It handles invalid inputs by displaying an error message and re-prompting the user. The function returns true
 *   if the user chooses 'Y' and false if the user chooses 'n'.
 */
_Bool _continue() {

	char choice;

	do {

		printf("Do you want to continue? [Y/n]: ");

		do {
			clearerr(stdin);
			choice = getchar();
		} while (choice == '\n');

		switch(tolower(choice)) {
	
			case 'y':
			case 'n':
				choice = tolower(choice);
				break;
			default:
				printf("Please choose correct option\n");
				choice = 'c';
				break;

		}

	} while (choice == 'c');

	return choice == 'y';

}
