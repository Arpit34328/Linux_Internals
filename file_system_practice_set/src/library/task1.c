/*
 * File Name: task1.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/questions.h"


/*
 * @brief Creates a file with a 4K byte hole
 *
 * This function creates a sparse file by:
 * 1. Writing data at the beginning
 * 2. Skipping 4096 bytes using lseek()
 * 3. Writing data after the hole
 */
void run_task1() {

	int fd;
	ssize_t bytes_written;
	const char *filename = "file_with_hole.txt";
	const char *start_data = "Start of the file\n";
	const char *end_data = "End of the file\n";

        printf("\n=== TASK 1: Create a file with a 4K byte hole ===\n\n");

        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
	if (fd == -1) {
        
		perror("Error opening file");
                return;
        
	}

        bytes_written = write(fd, start_data, strlen(start_data));
        
	if (bytes_written == -1) {
        
		perror("Error writing to file");
                close(fd);
        	return;
        
	}

        printf("Written %zd bytes at the beginning of the file\n", bytes_written);

        off_t offset = lseek(fd, 4096, SEEK_CUR);
        
	if (offset == -1) {
        
		perror("Error seeking in file");
                close(fd);
                return;
        
	}

        printf("Created a 4K byte hole (skipped 4096 bytes)\n");

        bytes_written = write(fd, end_data, strlen(end_data));
        
	if (bytes_written == -1) {
        
		perror("Error writing to file");
                close(fd);
                return;
        
	}

        printf("Written %zd bytes after the hole\n", bytes_written);

        if (close(fd) == -1) {
        
		perror("Error closing file");
                return;
        
	}

        struct stat file_stat;
        
	if (stat(filename, &file_stat) == -1) {
        
		perror("Error getting file stats");
                return;
        
	}

        printf("\nFile '%s' created successfully:\n", filename);
        printf("        - Apparent size: %ld bytes\n", file_stat.st_size);
        printf("        - Actual disk usage: %ld blocks (%ld bytes)\n",
               file_stat.st_blocks, file_stat.st_blocks * 512);
        printf("\nNote: The difference between apparent size and disk usage\n");
        printf("      shows the presence of the hole in the file.\n\n");

        printf("To verify the hole, you can use:\n");
        printf("        $ ls -ls %s\n", filename);
        printf("        $ hexdump -C %s\n\n", filename);

        printf("Press Enter to continue...");
        getchar();

}
