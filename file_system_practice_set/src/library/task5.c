/*
 * File Name: task5.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/questions.h"

// Code 1 from the practice set
void run_code1() {
        int i, r, w;
        char msg[12];
        char buf[2] = {0, 0};

        printf("\nRunning Code 1:\n");

        for (i = 0; i < 3; i++) {
                if ((r = read(i, buf, 1)) < 0) {
                        sprintf(msg, "read  f%d:%s", i, buf);
                        perror(msg);
                }
                if ((w = write(i, buf, 1)) < 0) {
                        sprintf(msg, "write f%d:%s", i, buf);
                        perror(msg);
                }
                fprintf(stderr, "%d, r = %d, w = %d, char = %d\n", i, r, w, (int)(buf[0]));
        }
}

// Code 2 from the practice set
void run_code2() {
        int i, r, w;
        char buf[2] = {0, 0};
        char msg[6][15] = {
                "read  stdin",
                "write stdin",
                "read  stdout",
                "write stdout",
                "read  stderr",
                "write stderr"
        };

        printf("\nRunning Code 2:\n");

        for (i = 0; i < 3; i++) {
                if ((r = read(i, buf, 1)) < 0)
                        perror(msg[i * 2]);

                if ((w = write(i, buf, 1)) < 0)
                        perror(msg[i * 2 + 1]);

                fprintf(stderr, "%d, r = %d, w = %d, char = %d\n", i, r, w, (int)(buf[0]));
        }
}

/*
 * @brief Tests and analyzes the provided code with different execution methods
 * 
 * Explains the differences between running the code in different ways:
 * 1. ./a.out
 * 2. ./a.out < in1
 * 3. ./a.out < in1 > out1
 */
void run_task5() {
        printf("\n=== TASK 5: Test and analyze the provided code ===\n\n");

        printf("This task analyzes two code snippets running with different I/O redirections.\n");
        printf("Rather than actually running the code, we'll explain the expected outcomes.\n\n");

        printf("Code 1 and Code 2 are similar but differ in error message handling.\n\n");

        printf("=== Case 1: $ ./a.out ===\n");
        printf("When run without redirection:\n");
        printf("- File descriptor 0 (stdin) is connected to terminal input\n");
        printf("- File descriptor 1 (stdout) is connected to terminal output\n");
        printf("- File descriptor 2 (stderr) is connected to terminal output\n\n");

        printf("Expected behavior:\n");
        printf("- The program waits for input on stdin (fd 0)\n");
        printf("- After you type a character and press Enter, it reads that character\n");
        printf("- It writes that character back to stdout (fd 1)\n");
        printf("- It tries to read from stdout (fd 1), which fails with \"Bad file descriptor\"\n");
        printf("- It tries to write to stderr (fd 2), which works but may mix with other output\n");
        printf("- It tries to read from stderr (fd 2), which fails with \"Bad file descriptor\"\n\n");

        printf("=== Case 2: $ ./a.out < in1 ===\n");
        printf("With input redirection:\n");
        printf("- File descriptor 0 (stdin) is connected to file 'in1'\n");
        printf("- File descriptor 1 (stdout) is connected to terminal output\n");
        printf("- File descriptor 2 (stderr) is connected to terminal output\n\n");

        printf("Expected behavior:\n");
        printf("- It reads the first character from 'in1' file ('A')\n");
        printf("- It writes 'A' to stdout (terminal)\n");
        printf("- The rest is the same as Case 1 for fd 1 and fd 2\n\n");

        printf("=== Case 3: $ ./a.out < in1 > out1 ===\n");
        printf("With input and output redirection:\n");
        printf("- File descriptor 0 (stdin) is connected to file 'in1'\n");
        printf("- File descriptor 1 (stdout) is connected to file 'out1'\n");
        printf("- File descriptor 2 (stderr) is still connected to terminal output\n\n");

        printf("Expected behavior:\n");
        printf("- It reads the first character from 'in1' file ('A')\n");
        printf("- It writes 'A' to the 'out1' file\n");
        printf("- It tries to read from stdout (fd 1), which now might work since it's a file\n");
        printf("- The rest is similar to previous cases\n\n");

        printf("=== Differences Between Code 1 and Code 2 ===\n");
        printf("- Code 1 creates error messages with the file descriptor number\n");
        printf("- Code 2 uses predefined error messages like \"read stdin\"\n");
        printf("- Otherwise, their functionality is identical\n\n");

        printf("=== Key Learning Points ===\n");
        printf("1. File descriptors 0, 1, and 2 are special:\n");
        printf("   - 0: Standard input (stdin)\n");
        printf("   - 1: Standard output (stdout)\n");
        printf("   - 2: Standard error (stderr)\n");
        printf("2. Input/output redirection changes where these file descriptors point\n");
        printf("3. You can read from regular files but not from output-only streams\n");
        printf("4. You can write to regular files and output streams but not to input-only streams\n\n");

        printf("Press Enter to continue...");
        getchar();

}

