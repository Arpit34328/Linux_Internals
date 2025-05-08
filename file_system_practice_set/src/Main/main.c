/*
 * File Name: main.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/p_set.h"

// External function that prompts user whether to continue or not
extern bool _continue();


/*
 * @brief Main function that runs the interactive file system practice set.
 *        Displays a menu, gets the user's question selection, runs the chosen
 *        question, and prompts whether to continue or exit.
 *
 * @return int Exit status.
 */
int main() {
    
	while (1) {
        
		system("clear");

		printf("+---------------------------------+\n");
		printf("| File System Practice Set Menu   |\n");
		printf("+---------------------------------+\n");

        
		display_question();
        
		uint8_t q_no = get_question();

		if (q_no == 0) break;

		run_question(q_no);

		if (!_continue()) break;
    
	}

   
       	return 0;

}
