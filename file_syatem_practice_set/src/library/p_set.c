/*
 * File Name: p_set.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>


#include "../include/p_set.h"
#include "../include/questions.h"

#define GARBAGE 0


/*
 * @brief Displays the list of available questions to run.
 */
void display_question() {

	printf("\nSelect question to run\n");

	printf("+-------------------------+\n");

    	printf("| Press 1 for \"qus1\"      |\n");

	printf("| Press 2 for \"qus2\"      |\n");

	printf("| Press 3 for \"qus3\"      |\n");

	printf("| Press 4 for \"qus4\"      |\n");

	printf("| Press 5 for \"qus5\"      |\n");

	printf("| Press 6 for \"qus6\"      |\n");
	
	printf("| Press 7 for \"qus7\"      |\n");
	
	printf("| Press 8 for \"qus8\"      |\n");

	printf("| Press 9 for \"qus9\"      |\n");

	printf("| Press 0 to \"Exit\"       |\n");

	printf("+-------------------------+\n");

}


/*
 * @brief Prompts the user to select a valid question (1 to 5). Retries until valid input is given.
 *
 * @return uint8_t The selected question number.
 */
uint8_t get_question() {

	char input[10];

	static bool garbage = GARBAGE;

	while (1) {

		printf("Which question do you want to run? [1-9]: ");
	
		// Garbage Handle
        	if (garbage) {

                	getchar();
                	clearerr(stdin);

       		}

        	garbage = !GARBAGE;		


		if (fgets(input, sizeof(input), stdin) == NULL) {

			perror("Input Error");
            		continue;

		}

		input[strcspn(input, "\n")] = '\0';

        	int choice = atoi(input);

        	if (choice >= 0 && choice <= 9) {

			return (uint8_t)choice;

		} 
		else {

			fprintf(stderr, "Error: Invalid choice '%s'. Please enter a number between 1 and 5.\n", input);

		}

	}
}


/*
 * @brief Runs the code for the selected question number.
 *
 * @param q_no Question number from 1 to 5.
 */
void run_question(uint8_t q_no) {
    
	switch (q_no) {

		case 1:
            
			run_task1();
			break;
        
		case 2:
           
           		run_task2(); 
			break;
        
		case 3:
            
         		run_task3(); 
		       	break;
        
		case 4:
            
      			run_task4(); 
			break;
        
		case 5:
            
           		run_task5(); 
			break;
       
		case 6:

			run_task6();
                        break;

                case 7:

	//		run_task7();
                        break;

                case 8:

                        printf("Running Question 8\n");

                        break;

                case 9:

                        printf("Running Question 9\n");

                        break;

		default:
            
			fprintf(stderr, "Error: Invalid question number %d\n", q_no);
            
			break;
    
	}

}

