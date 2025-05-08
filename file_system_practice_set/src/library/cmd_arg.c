/*
 * File Name: cmd_arg.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "../include/cmd_arg.h"

uint8_t argc;
char *argv[5];

//#define GARBAGE 0

/*
 * @brief Reads and tokenizes a command-line-style input from the user.
 *        It simulates command-line arguments and stores them in the argv array.
 *        Also validates the first argument to ensure it's './a.out'.
 */
void get_cmd_arg() {

	argc = 0;

	char arg[100];

	//static bool garbage = GARBAGE;

	printf("\033[1;32marpit343@HLQDX33\033[0m:\033[1;34m~/166967_Arpit_Dennis_Ritchee\033[0m$ ");


	// Garbage Handle
	/*
	if (garbage) {
	
		getchar();
		clearerr(stdin);
	
	}

	garbage = !GARBAGE;
	*/

	if (fgets(arg, sizeof(arg), stdin) == NULL) {
		
		perror("Input Error");
		return;

	}

	char *token = strtok(arg, " \n");

	while (token != NULL && argc < 5) {
	
		argv[argc] = malloc(strlen(token) + 1);
		if (argv[argc] == NULL) {
			perror("Memory Allocation Error");
			free_arg();
			return;
		}
		strcpy(argv[argc], token);
		argc++;
		token = strtok(NULL, " \n");

	}	

	if (argc == 0) {

		fprintf(stderr, "Error: No argument found\n");
		return;

	}

	assert(argv[0] != NULL);

	if (strcmp(argv[0], "./a.out") != 0) {

		fprintf(stderr, "Error: %s file not found\n", argv[0]);
		free_arg();
		return;

	}

}	


/*
 * @brief Frees all memory allocated to the argv array.
 */
void free_arg() {

	for (uint8_t index = 0; index < argc; index++) {

		free(argv[index]);
		argv[index] = NULL;

	}

	argc = 0;

}
