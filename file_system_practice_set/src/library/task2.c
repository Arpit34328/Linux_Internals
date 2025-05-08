/*
 * File Name: task2.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#include "../include/questions.h"
#include "../include/cmd_arg.h"


#define BUFFER_SIZE 4096


/*
 * @brief Copies contents from one file to another
 *
 * Uses command line arguments for source and destination files
 * Format: ./a.out source_file destination_file
 */
void run_task2() {

	extern uint8_t argc;
	extern char *argv[];
	int source_fd, dest_fd;
	ssize_t bytes_read, bytes_written;
	char buffer[BUFFER_SIZE];

	printf("\n=== TASK 2: Copy file contents ===\n\n");
	printf("Usage: ./a.out source_file destination_file\n\n");


	get_cmd_arg();

	if (argc != 3) {
	
		fprintf(stderr, "Error: Expected 2 arguments (source and destination files)\n");
        	fprintf(stderr, "Usage: ./a.out source_file destination_file\n");
        	free_arg();
        	return;

	}

       	source_fd = open(argv[1], O_RDONLY);
        
	if (source_fd == -1) {
        
		fprintf(stderr, "Error opening source file '%s': %s\n",
                        argv[1], strerror(errno));
        	free_arg();
        	return;
        
	}

      	dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
	if (dest_fd == -1) {
       	
		fprintf(stderr, "Error opening destination file '%s': %s\n",
                        argv[2], strerror(errno));
      		close(source_fd);
       		free_arg();
       		return;
        
	}

      	printf("Copying from '%s' to '%s'...\n", argv[1], argv[2]);

       	size_t total_bytes = 0;
        
	while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        
		bytes_written = write(dest_fd, buffer, bytes_read);

       		if (bytes_written == -1) {
        
			fprintf(stderr, "Error writing to destination file: %s\n",
                                strerror(errno));
       			close(source_fd);
        		close(dest_fd);
        		free_arg();
       			return;
       
		}

        	if (bytes_written != bytes_read) {
        
			fprintf(stderr, "Warning: Partial write occurred\n");
        
		}

                total_bytes += bytes_written;
        
	}

        if (bytes_read == -1) {
        
		fprintf(stderr, "Error reading from source file: %s\n",
                        strerror(errno));
        	close(source_fd);
        	close(dest_fd);
        	free_arg();
        	return;
        
	}

        if (close(source_fd) == -1) {
        
		fprintf(stderr, "Error closing source file: %s\n", strerror(errno));
        
	}

        
	if (close(dest_fd) == -1) {
        
		fprintf(stderr, "Error closing destination file: %s\n", strerror(errno));
        
	}

        printf("Copy completed successfully. Copied %zu bytes.\n\n", total_bytes);

        free_arg();

        printf("Press Enter to continue...");
        getchar();

}

