/*
 * File Name: task8.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "../include/questions.h"
#include "../include/cmd_arg.h"

typedef struct {
        char name[256];
        off_t size;
        mode_t mode;
} file_info_t;

int compare_names(const void *a, const void *b) {
  
  	return strcmp(((file_info_t *)a)->name, ((file_info_t *)b)->name);

}

char get_file_annotation(mode_t mode) {

	if (S_ISDIR(mode)) return '/';
        if (S_ISLNK(mode)) return '@';
        if (mode & S_IXUSR) return '*';
        return '\0';

}

/*
 * @brief Simple ls implementation: prints file names and sizes in a directory
 */
void simple_ls(const char *path) {
  
  	DIR *dir = opendir(path);
        if (!dir) {
  
  		perror("opendir");
                return;
  
  	}

        printf("\nSimple ls implementation for directory: %s\n\n", path);
        printf("%-30s %15s\n", "Filename", "Size (bytes)");
        printf("%-30s %15s\n", "--------", "------------");

        int capacity = 10, file_count = 0;
        file_info_t *files = malloc(capacity * sizeof(file_info_t));
        if (!files) {
  
  		perror("malloc");
                closedir(dir);
  		return;
  
  	}

        struct dirent *entry;
        struct stat statbuf;

        while ((entry = readdir(dir)) != NULL) {
  
  		char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

                if (stat(full_path, &statbuf) == -1) {
  
  			perror("stat");
                        continue;
  
  		}

                if (file_count >= capacity) {
  
  			capacity *= 2;
                        file_info_t *new_files = realloc(files, capacity * sizeof(file_info_t));
                        if (!new_files) {
  
  				perror("realloc");
                                break;
  
  			}
                        files = new_files;
  
  		}

                strncpy(files[file_count].name, entry->d_name, sizeof(files[file_count].name) - 1);
                files[file_count].name[sizeof(files[file_count].name) - 1] = '\0';
                files[file_count].size = statbuf.st_size;
                files[file_count].mode = statbuf.st_mode;
  		file_count++;

  	}

        closedir(dir);

        qsort(files, file_count, sizeof(file_info_t), compare_names);

        for (int i = 0; i < file_count; i++) {
        
		printf("%-30s %15ld\n", files[i].name, files[i].size);
        
	}

        free(files);

}

/*
 * @brief ls -F implementation: adds file-type indicators and prints types
 */
void ls_with_annotations(const char *path) {
  
  	DIR *dir = opendir(path);
  
  	if (!dir) {
  
  		perror("opendir");
                return;
  
  	}

        printf("\nls -F implementation for directory: %s\n\n", path);
        printf("%-30s %15s %s\n", "Filename", "Size (bytes)", "Type");
        printf("%-30s %15s %s\n", "--------", "------------", "----");

        int capacity = 10, file_count = 0;
  
  	file_info_t *files = malloc(capacity * sizeof(file_info_t));
  
  	if (!files) {
 
 		perror("malloc");
  		closedir(dir);
                return;
        
	}

        struct dirent *entry;
        struct stat statbuf;

        while ((entry = readdir(dir)) != NULL) {
        
		char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

                if (lstat(full_path, &statbuf) == -1) {
        
			perror("lstat");
                        continue;
        
		}

                if (file_count >= capacity) {
        
			capacity *= 2;
                        file_info_t *new_files = realloc(files, capacity * sizeof(file_info_t));
        
			if (!new_files) {
        
				perror("realloc");
                                break;
        
			}
                        files = new_files;
        
		}

                strncpy(files[file_count].name, entry->d_name, sizeof(files[file_count].name) - 1);
                files[file_count].name[sizeof(files[file_count].name) - 1] = '\0';
                files[file_count].size = statbuf.st_size;
                files[file_count].mode = statbuf.st_mode;
                file_count++;
        
	}

        closedir(dir);

        qsort(files, file_count, sizeof(file_info_t), compare_names);

        for (int i = 0; i < file_count; i++) {
        
		char annotation = get_file_annotation(files[i].mode);
        
		if (annotation) {
        
			printf("%-29s%c %15ld ", files[i].name, annotation, files[i].size);
                } 
		else {
                
			printf("%-30s %15ld ", files[i].name, files[i].size);
                
		}

                if (S_ISREG(files[i].mode)) {
                
			printf("Regular file");
                        if (files[i].mode & S_IXUSR) {
                
				printf(" (executable)");
                
			}
                } 
		else if (S_ISDIR(files[i].mode)) {
                
			printf("Directory");
        
		} 
		else if (S_ISLNK(files[i].mode)) {
                
			printf("Symbolic link");
        
		} 
		else if (S_ISFIFO(files[i].mode)) {
                
			printf("FIFO/pipe");
                
		} 
		else if (S_ISSOCK(files[i].mode)) {
                
			printf("Socket");
                
		} 
		else if (S_ISBLK(files[i].mode)) {
                
			printf("Block device");
               
	       	}
	       	else if (S_ISCHR(files[i].mode)) {
               
	       		printf("Character device");
               
	       	} 
		else {
                
			printf("Unknown");
                
		}

                printf("\n");
        
	}

        free(files);

}


/*
 * @brief Recursively calculate directory size, skipping symlinks
 */
int calculate_dir_size(const char *path, off_t *total_size) {
  
  	DIR *dir = opendir(path);
        if (!dir) {
  
  		perror("opendir");
                return -1;
  
  	}

        struct dirent *entry;
        struct stat statbuf;

        while ((entry = readdir(dir)) != NULL) {
  
  		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
  
  			continue;
  
  		}

                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

                if (lstat(full_path, &statbuf) == -1) {
  
  			perror("lstat");
                        continue;
  
  		}

                if (S_ISREG(statbuf.st_mode)) {
  
  			*total_size += statbuf.st_size;
  
  		} 
		else if (S_ISDIR(statbuf.st_mode)) {
                
			if (calculate_dir_size(full_path, total_size) == -1) {
                
				closedir(dir);
                                return -1;
                
			}
                
		}
        }

        closedir(dir);
        return 0;

}


/*
 * @brief Calculates total size of files (non-recursive)
 */
void calculate_total_size(const char *path) {
  
  	DIR *dir = opendir(path);
  
  	if (!dir) {
  
  		perror("opendir");
  
  		return;
  
  	}

        printf("\nCalculating total size of files in: %s\n\n", path);

        struct dirent *entry;
        struct stat statbuf;
        off_t total_size = 0;

        while ((entry = readdir(dir)) != NULL) {
  
  		char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

                if (lstat(full_path, &statbuf) == -1) {
  
  			perror("lstat");
                        continue;
  
  		}

                if (S_ISREG(statbuf.st_mode)) {
  
  			total_size += statbuf.st_size;
  
  		}
        }

        closedir(dir);

        printf("Total size: %ld bytes (%.2f KB, %.2f MB)\n", 
               total_size, 
               total_size / 1024.0, 
               total_size / (1024.0 * 1024.0));

}


/*
 * @brief Calculates total size of files recursively
 */
void calculate_recursive_size(const char *path) {
  
  	printf("\nCalculating recursive size of files in: %s\n\n", path);

        off_t total_size = 0;
  
  	if (calculate_dir_size(path, &total_size) == -1) {
  
  		printf("Error calculating directory size\n");
                return;
  
  	}

        printf("Total size (recursive): %ld bytes (%.2f KB, %.2f MB)\n", 
               total_size, 
               total_size / 1024.0, 
               total_size / (1024.0 * 1024.0));

}

/**
 * @brief Main function for Task 8: ls variants
 */
void run_task8() {

	extern uint8_t argc;
        extern char *argv[];

        printf("\n=== TASK 8: Implement ls command variants ===\n\n");
        printf("Options:\n");
        printf("  ./a.out ls [directory_path]            - Basic ls implementation\n");
        printf("  ./a.out ls-F [directory_path]          - ls -F implementation\n");
        printf("  ./a.out size [directory_path]          - Calculate total size\n");
        printf("  ./a.out recursive-size [directory_path] - Calculate recursive size\n\n");

        get_cmd_arg();

        const char *dir_path = ".";

        if (argc < 2) {

		printf("No command specified. Defaulting to basic ls.\n");
                simple_ls(dir_path);
                free_arg();
                return;

	}

        if (argc >= 3) {

		dir_path = argv[2];

	}

        if (strcmp(argv[1], "ls") == 0) {

		simple_ls(dir_path);

	} 
	else if (strcmp(argv[1], "ls-F") == 0) {
        
		ls_with_annotations(dir_path);
       
       	}
      	else if (strcmp(argv[1], "size") == 0) {
      
      		calculate_total_size(dir_path);
      
      	} 
	else if (strcmp(argv[1], "recursive-size") == 0) {
        
		calculate_recursive_size(dir_path);
        
	} 
	else {

		printf("Unknown command: %s\n", argv[1]);
                printf("Available commands: ls, ls-F, size, recursive-size\n");

	}

        free_arg();

        printf("\nPress Enter to continue...");
        getchar();

}

