/*
 * File Name: task7.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "../include/questions.h"
#include "../include/cmd_arg.h"

#define BUFFER_SIZE 4096

/*
 * @brief Implementation of cat command using system calls
 * 
 * @param filename Name of the file to read from, or NULL for stdin
 */
void cat_syscall(const char *filename) {
        
	int fd;
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read, bytes_written;

        if (filename != NULL) {
                fd = open(filename, O_RDONLY);
                if (fd == -1) {
                        perror("Error opening file");
                        return;
                }
        } 
	else {
                fd = STDIN_FILENO;
        }

        while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
                bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
                if (bytes_written != bytes_read) {
                        perror("Error writing to stdout");
                        if (filename != NULL) {
                                close(fd);
                        }
                        return;
                }
        }

        if (bytes_read == -1) {
                perror("Error reading from file");
        }

        if (filename != NULL) {
                if (close(fd) == -1) {
                        perror("Error closing file");
                }
        }
}


/*
 * @brief Implementation of cat command using standard I/O library
 * 
 * @param filename Name of the file to read from, or NULL for stdin
 */
void cat_stdio(const char *filename) {
        
	FILE *file;
        char buffer[BUFFER_SIZE];
        size_t bytes_read;

        if (filename != NULL) {
        
		file = fopen(filename, "r");
        
		if (file == NULL) {
        
			perror("Error opening file");
                        return;
        
		}
        
	} 
	else {
        
		file = stdin;
        
	}

        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
                if (fwrite(buffer, 1, bytes_read, stdout) != bytes_read) {
                        perror("Error writing to stdout");
                        if (filename != NULL) {
                                fclose(file);
                        }
                        return;
                }
        }

        if (ferror(file)) {
                perror("Error reading from file");
        }

        if (filename != NULL) {
                if (fclose(file) != 0) {
                        perror("Error closing file");
                }
        }
}

/**
 * @brief Full implementation of cat command supporting multiple files and stdin
 */
void cat_full() {
        
	extern uint8_t argc;
        extern char *argv[];
        int i;

        if (argc <= 1) {
                cat_stdio(NULL);
                return;
        }

        for (i = 1; i < argc; i++) {
                cat_stdio(argv[i]);
        }

}

/**
 * @brief Answers questions about standard I/O and implements cat
 */
void run_task7() {
        
	extern uint8_t argc;
        extern char *argv[];

        printf("\n=== TASK 7: Answer questions about standard I/O and implement cat ===\n\n");

        printf("Usage options:\n");
        printf("1. ./a.out syscall [filename]   - Use system calls\n");
        printf("2. ./a.out stdio [filename]     - Use standard I/O\n");
        printf("3. ./a.out full [file1 file2...] - Full cat implementation\n");
        printf("4. ./a.out questions            - Standard I/O analysis\n");
        printf("5. ./a.out bugfix               - Bugfixed getchar implementation\n\n");

        get_cmd_arg();

        if (argc < 2 || (strcmp(argv[1], "syscall") != 0 && 
                         strcmp(argv[1], "stdio") != 0 && 
                         strcmp(argv[1], "full") != 0 &&
                         strcmp(argv[1], "questions") != 0 &&
                         strcmp(argv[1], "bugfix") != 0)) {

                printf("=== Standard I/O Library Analysis ===\n\n");

                printf("1. What can we infer about the standard I/O library?\n");
                printf("   - The standard I/O library provides buffering that reduces system calls\n");
                printf("   - It handles buffering automatically, making code simpler\n");
                printf("   - It's generally more efficient for character-by-character operations\n");
                printf("   - It provides higher-level abstractions (fprintf, fscanf, etc.)\n");
                printf("   - The library manages its own internal buffers for efficiency\n\n");

                printf("2. Why is getchar() faster than fread(c, 1, 1, stdin)?\n");
                printf("   - getchar() is usually implemented as a macro that directly accesses\n");
                printf("     the stdin buffer and only makes a function call when the buffer is empty\n");
                printf("   - fread() has additional overhead for handling its more general interface\n");
                printf("   - getchar() is specifically optimized for single-character reading\n\n");

                printf("3. How to cut down on too many system calls?\n");
                printf("   - Use larger buffer sizes when reading/writing\n");
                printf("   - Use standard I/O functions which do internal buffering\n");
                printf("   - Set appropriate buffer modes (line buffered, fully buffered)\n");
                printf("   - Avoid character-by-character I/O operations\n");
                printf("   - Use memory-mapped I/O for large files (mmap)\n\n");

                printf("=== Bug in getchar() Implementation ===\n\n");
                printf("The bug in the simplified getchar() implementation:\n");
                printf("1. When read() returns 0 (EOF), n is decremented to -1\n");
                printf("2. In subsequent calls, n is negative, which causes:\n");
                printf("   - The buffer is not refilled\n");
                printf("   - The function returns EOF\n");
                printf("3. This is correct, but n will keep decreasing on each call\n");
                printf("   which could eventually cause an integer overflow\n\n");

                printf("Fixed implementation:\n");
                printf("int getchar(void) {\n");
                printf("        static char buf[BUFSIZE];\n");
                printf("        static char *bufp;\n");
                printf("        static int  n = 0;\n");
                printf("\n");
                printf("        if (n == 0) {\n");
                printf("                n = read(0, buf, BUFSIZE);\n");
                printf("                bufp = buf;\n");
                printf("                if (n <= 0) return EOF;  // Return EOF immediately if no data\n");
                printf("        }\n");
                printf("\n");
                printf("        n--;\n");
                printf("        return (unsigned char)(*bufp++);  // Cast to unsigned char to handle\n");
                printf("                                          // all 8-bit values correctly\n");
                printf("}\n");
        
	} 
	else if (strcmp(argv[1], "syscall") == 0) {
                
                if (argc >= 3) {
                
			cat_syscall(argv[2]);
        
		} 
		else {
        
			cat_syscall(NULL);
        
		}
        
	} 
	else if (strcmp(argv[1], "stdio") == 0) {
                
		if (argc >= 3) {
                
			cat_stdio(argv[2]);
                
		} 
		else {
        
			cat_stdio(NULL);
        
		}
        
	} 
	else if (strcmp(argv[1], "full") == 0) {
       
       		cat_full();
        
	} 
	else if (strcmp(argv[1], "bugfix") == 0) {

		printf("=== Fixed getchar() Implementation ===\n\n");
                printf("int getchar(void) {\n");
                printf("        static char buf[BUFSIZE];\n");
                printf("        static char *bufp;\n");
                printf("        static int  n = 0;\n");
                printf("\n");
                printf("        if (n == 0) {\n");
                printf("                n = read(0, buf, BUFSIZE);\n");
                printf("                bufp = buf;\n");
                printf("                if (n <= 0) return EOF;  // Return EOF immediately if no data\n");
                printf("        }\n");
                printf("\n");
                printf("        n--;\n");
                printf("        return (unsigned char)(*bufp++);  // Cast to unsigned char to handle\n");
                printf("                                          // all 8-bit values correctly\n");
                printf("}\n");

	}

        free_arg();

	printf("\nPress Enter to continue...");

	getchar();

}

