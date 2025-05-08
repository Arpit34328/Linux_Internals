/*
 * File Name: task6.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 * Description: Implements and analyzes different versions of the 'cat' command
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "../include/questions.h"
#include "../include/cmd_arg.h"


/*
 * @brief Version 1: Using getchar/putchar
 */
void run_simpcat1() {

	printf("\nRunning simpcat1 (getchar/putchar version):\n");
        printf("Reading from stdin, writing to stdout...\n");

        char c;
        clock_t start = clock();

        while ((c = getchar()) != EOF) {
                putchar(c);
        }

        clock_t end = clock();
        fprintf(stderr, "\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

}


/*
 * @brief Version 2: Using system calls read/write (1 byte at a time)
 */
void run_simpcat2() {

	printf("\nRunning simpcat2 (read/write version):\n");
        printf("Reading from stdin, writing to stdout...\n");

        char c;
        int bytes;
        clock_t start = clock();

        while ((bytes = read(0, &c, 1)) > 0) {
                write(1, &c, 1);
        }

        clock_t end = clock();
        fprintf(stderr, "\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

}


/*
 * @brief Version 3: Using fread/fwrite (1 byte at a time)
 */
void run_simpcat3() {

	printf("\nRunning simpcat3 (fread/fwrite version):\n");
        printf("Reading from stdin, writing to stdout...\n");

        char c;
        size_t bytes;
        clock_t start = clock();

        while ((bytes = fread(&c, 1, 1, stdin)) > 0) {
                fwrite(&c, 1, 1, stdout);
        }

        clock_t end = clock();
	fprintf(stderr, "\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

}


/*
 * @brief Version 4: Using read/write with custom buffer
 */
void run_simpcat4(int bufsize) {

	printf("\nRunning simpcat4 (read/write with buffer size %d):\n", bufsize);
        printf("Reading from stdin, writing to stdout...\n");

        char *buffer = malloc(bufsize);
        if (!buffer) {
                perror("Memory allocation failed");
                return;
        }

        int bytes;
        clock_t start = clock();

        while ((bytes = read(0, buffer, bufsize)) > 0) {
                write(1, buffer, bytes);
        }

        clock_t end = clock();
        fprintf(stderr, "\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

        free(buffer);

}

/*
 * @brief Version 5: Using fread/fwrite with custom buffer
 */
void run_simpcat5(int bufsize) {
  
  	printf("\nRunning simpcat5 (fread/fwrite with buffer size %d):\n", bufsize);
        printf("Reading from stdin, writing to stdout...\n");

        char *buffer = malloc(bufsize);
        if (!buffer) {
                perror("Memory allocation failed");
                return;
        }

        size_t bytes;
        clock_t start = clock();

        while ((bytes = fread(buffer, 1, bufsize, stdin)) > 0) {
                fwrite(buffer, 1, bytes, stdout);
        }

        clock_t end = clock();
        fprintf(stderr, "\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

        free(buffer);

}


/*
 * @brief Analysis and runner for task6
 */
void run_task6() {
        
	extern uint8_t argc;
        extern char *argv[];

        printf("\n=== TASK 6: Implement and analyze cat command variants ===\n\n");
        printf("This task demonstrates different implementations of the 'cat' command\n");
        printf("and analyzes their performance with different buffering strategies.\n\n");

        printf("Usage:\n");
        printf("  ./a.out simpcat1\n");
        printf("  ./a.out simpcat2\n");
        printf("  ./a.out simpcat3\n");
        printf("  ./a.out simpcat4 <buffer_size>\n");
        printf("  ./a.out simpcat5 <buffer_size>\n");
        printf("  ./a.out analysis\n\n");

        get_cmd_arg();

        if (argc < 2) {
        
		printf("No arguments provided. Defaulting to analysis mode.\n\n");
                goto analysis;
        
	}

        if (strcmp(argv[1], "simpcat1") == 0) {
        
		run_simpcat1();
        
	} 
	else if (strcmp(argv[1], "simpcat2") == 0) {
        
		run_simpcat2();
        
	} 
	else if (strcmp(argv[1], "simpcat3") == 0) {
        
		run_simpcat3();
        
	} 
	else if (strcmp(argv[1], "simpcat4") == 0) {
       
       		int bufsize = (argc >= 3) ? atoi(argv[2]) : 4096;
       		run_simpcat4(bufsize);
       
       	}
       	else if (strcmp(argv[1], "simpcat5") == 0) {
        
		int bufsize = (argc >= 3) ? atoi(argv[2]) : 4096;
                run_simpcat5(bufsize);
        
	} 
	else if (strcmp(argv[1], "analysis") == 0) {
        
	analysis:
                printf("=== Performance Analysis ===\n\n");

                printf("1. simpcat1 (getchar/putchar):\n");
                printf("   - Uses stdio character I/O\n");
                printf("   - Moderate performance due to internal buffering\n\n");

                printf("2. simpcat2 (read/write):\n");
                printf("   - One syscall per byte\n");
                printf("   - Least efficient due to syscall overhead\n\n");

                printf("3. simpcat3 (fread/fwrite):\n");
                printf("   - Uses stdio buffering internally\n");
                printf("   - Better than raw syscalls, but still per-byte logic\n\n");

                printf("4. simpcat4 (read/write with buffer):\n");
                printf("   - Manual buffer reduces syscalls\n");
                printf("   - Very efficient with buffers around 4KB\n\n");

                printf("5. simpcat5 (fread/fwrite with buffer):\n");
                printf("   - Best of both worlds: stdio buffering + large blocks\n");
                printf("   - Often the fastest version\n\n");

                printf("General notes:\n");
                printf("- Use page-sized buffers (e.g., 4096 bytes) for best performance\n");
                printf("- Extremely large buffers (>64KB) give diminishing returns\n");
                printf("- Read/write is lower-level but provides control over performance\n");
                printf("- Stdio is easier to use and handles buffering automatically\n\n");
        
	} 
	else {

		printf("Unknown command: %s\n", argv[1]);

	}

        free_arg();

        printf("\nPress Enter to continue...");
        getchar();

}

