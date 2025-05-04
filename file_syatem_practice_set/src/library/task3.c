/*
 * File Name: task3.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#include "../include/questions.h"
#include "../include/cmd_arg.h"


/*
 * @brief Determines and returns the file type as a string
 * 
 * @param path Full path to the file
 * @return const char* String describing the file type
 */
const char* get_file_type(const char *path) {
        
	struct stat file_stat;

        if (lstat(path, &file_stat) == -1) {
        
		return "Unknown (error)";
        
	}

        if (S_ISREG(file_stat.st_mode)) {
        
		return "Regular file";
        
	} 
	else if (S_ISDIR(file_stat.st_mode)) {
       
       		return "Directory";
       
       	}
       	else if (S_ISLNK(file_stat.st_mode)) {
        
		return "Symbolic link";
        
	} 
	else if (S_ISCHR(file_stat.st_mode)) {
        
		return "Character device";
        
	} 
	else if (S_ISBLK(file_stat.st_mode)) {
       
       		return "Block device";
       
       	}
       	else if (S_ISFIFO(file_stat.st_mode)) {
        
		return "FIFO/pipe";
        
	} 
	else if (S_ISSOCK(file_stat.st_mode)) {
        
		return "Socket";
        
	} 
	else {

		return "Unknown";

	}

}



/*
 * @brief Displays the contents of a directory with file types
 * 
 * Uses command line argument for the directory path.
 * Format: ./a.out [directory_path]
 * If no argument is provided, uses the current directory.
 */
void run_task3() {
        
	extern uint8_t argc;
        extern char *argv[];
        DIR *dir;
        struct dirent *entry;
        char path[1024];

        printf("\n=== TASK 3: Display directory contents with file types ===\n\n");
        printf("Usage: ./a.out [directory_path]\n");
        printf("If no directory path is provided, the current directory will be used.\n\n");

        get_cmd_arg();

        const char *dir_path;
        
	if (argc < 2) {
        
		dir_path = ".";
                printf("No directory specified. Using current directory.\n\n");
        
	} 
	else {
 
 		dir_path = argv[1];
                printf("Directory specified: %s\n\n", dir_path);

	}

	dir = opendir(dir_path);

	if (dir == NULL) {

		fprintf(stderr, "Error opening directory '%s': %s\n",
                        dir_path, strerror(errno));
                free_arg();
		return;

	}

        printf("%-30s %-20s %-12s\n", "Name", "Type", "Size (bytes)");
        printf("%-30s %-20s %-12s\n", "----", "----", "------------");

        while ((entry = readdir(dir)) != NULL) {

		struct stat file_stat;

		snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        
		if (lstat(path, &file_stat) == -1) {
        
			fprintf(stderr, "Error getting stats for '%s': %s\n",
                                entry->d_name, strerror(errno));
        		continue;
        
		}

                printf("%-30s %-20s %-12ld\n",
                       entry->d_name,
                       get_file_type(path),
                       file_stat.st_size);
        
	}

        if (closedir(dir) == -1) {
        
		fprintf(stderr, "Error closing directory: %s\n", strerror(errno));
        
	}

        printf("\n");

	free_arg();

	printf("Press Enter to continue...");

	getchar();

}

