/*
 * File Name: task4.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "../include/questions.h"

/*
 * @brief Fixes and tests the given code with appropriate flags
 * 
 * The original code had a missing flag in lseek. This function fixes that and
 * adds appropriate error checking.
 */
void run_task4() {
        
	char *c;
        
	int fd, sz, i;

        printf("\n=== TASK 4: Fix and test code with appropriate flags ===\n\n");

        FILE *test_file = fopen("in1", "w");
        
	if (test_file == NULL) {
        
		perror("Error creating test file");
                return;
        
	}
        
	fprintf(test_file, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        fclose(test_file);

        printf("Created test file 'in1' with content: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\n");
        printf("Running the fixed code...\n\n");

        c = (char *) calloc(100, sizeof(char));
        
	if (c == NULL) {
        
		perror("Memory allocation error");
                return;
        
	}

        fd = open("in1", O_RDONLY);
        
	if (fd < 0) {
        
		perror("Error opening file");
                free(c);
                return;
        
	}

        sz = read(fd, c, 10);
        
	if (sz < 0) {
                
		perror("Error reading file");
                close(fd);
       		free(c);
       		return;
       }

        printf("We have opened in1, and called read(%d, c, 10).\n", fd);
        printf("It returned that %d bytes were read.\n", sz);
        c[sz] = '\0';
        printf("Those bytes are as follows: %s\n", c);

        i = lseek(fd, 0, SEEK_CUR);
        
	if (i < 0) {
        
		perror("Error in lseek");
                close(fd);
                free(c);
    		return;
    
	}

        printf("lseek(%d, 0, SEEK_CUR) returns that the current offset of the file is %d\n\n", fd, i);

        printf("now, we seek to the beginning of the file and call read(%d, c, 10)\n", fd);
        
	if (lseek(fd, 0, SEEK_SET) < 0) {
        
		perror("Error in lseek");
        	close(fd);
        	free(c);
        	return;
        
	}

        sz = read(fd, c, 10);
        
	if (sz < 0) {
        
		perror("Error reading file");
        	close(fd);
       		free(c);
        	return;
        
	}

      	c[sz] = '\0';
        
	printf("The read returns the following bytes: %s\n", c);

        printf("now, we do lseek(%d, -6, SEEK_CUR).  It returns %d\n",
               fd, lseek(fd, -6, SEEK_CUR));

        printf("If we do read(%d, c, 10), we get the following bytes: ", fd);

     	sz = read(fd, c, 10);
        
	if (sz < 0) {
        
		perror("Error reading file");
                close(fd);
                free(c);
        	return;
        
	}

	c[sz] = '\0';
        
	printf("%s\n", c);

      	printf("Finally, we try lseek(%d, -1, SEEK_SET). ", fd);

        i = lseek(fd, -1, SEEK_SET);
      
      	if (i < 0) {
       
       		printf("This returns -1.\n");
      		printf("perror() tells us why: ");
      		perror("");
       
       	} 
	else {
	
		printf("This returned %d (which is unexpected - should fail).\n", i);

	}

	close(fd);
	free(c);

	printf("\nExplanation of fixes:\n");
	printf("1. Added proper error checking throughout the code\n");
	printf("2. Fixed the undefined 'flag' variable:\n");
	printf("   - Used SEEK_CUR when checking current position\n");
	printf("   - Used SEEK_SET when seeking to beginning of file\n");
	printf("   - Used SEEK_CUR for relative seeking\n");
	printf("3. Added proper memory allocation and file handling\n\n");

	printf("Press Enter to continue...");

	getchar();

}

