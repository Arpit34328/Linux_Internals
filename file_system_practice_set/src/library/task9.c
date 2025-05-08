/*
 * File Name: task9.c
 * Author: Arpit Patel
 * Created: 05 May, 2025
 * Description: Implements proc filesystem exploration with SystemInfo and ProcessInfo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/questions.h"
#include "../include/cmd_arg.h"

#define BUFFER_SIZE 8192
#define LINE_SIZE 1024
#define MAX_ENV_SIZE 4096

/*
 * @brief Gets the number of CPUs and their information
 */
void get_cpu_info() {
        FILE *cpu_file;
        char line[LINE_SIZE];
        int cpu_count = 0;
        float clock_speed;
        int cores = 0;
        
        printf("\n1. CPU Information:\n");
        printf("------------------\n");
        
        cpu_file = fopen("/proc/cpuinfo", "r");
        if (cpu_file == NULL) {
                perror("Error opening /proc/cpuinfo");
                return;
        }
        
        while (fgets(line, sizeof(line), cpu_file) != NULL) {
                if (strncmp(line, "processor", 9) == 0) {
                        cpu_count++;
                }
                
                if (strncmp(line, "cpu MHz", 7) == 0) {
                        char *freq = strchr(line, ':');
                        if (freq) {
                                sscanf(freq + 1, "%f", &clock_speed);
                                printf("    CPU #%d clock speed: %.2f MHz\n", cpu_count, clock_speed);
                        }
                }
                
                if (strncmp(line, "cpu cores", 9) == 0) {
                        char *core_info = strchr(line, ':');
                        if (core_info) {
                                sscanf(core_info + 1, "%d", &cores);
                        }
                }
        }
        
        fclose(cpu_file);
        printf("    Total CPUs: %d\n", cpu_count);
        
        if (cores > 0) {
                printf("    Cores per CPU: %d\n", cores);
                printf("    Total cores: %d\n", cores * (cpu_count / cores));
        }
}

/*
 * @brief Gets the Linux kernel version
 */
void get_kernel_version() {
        FILE *version_file;
        char version[LINE_SIZE];
        
        printf("\n2. Linux Kernel Version:\n");
        printf("----------------------\n");
        
        version_file = fopen("/proc/version", "r");
        if (version_file == NULL) {
                perror("Error opening /proc/version");
                return;
        }
        
        if (fgets(version, sizeof(version), version_file) != NULL) {
                // Extract just the version number
                char *ver_start = strstr(version, "version ");
                if (ver_start) {
                        ver_start += 8; // Move past "version "
                        char *ver_end = strchr(ver_start, ' ');
                        if (ver_end) {
                                *ver_end = '\0';
                                printf("    Kernel version: %s\n", ver_start);
                        } else {
                                printf("    %s", version);
                        }
                } else {
                        printf("    %s", version);
                }
        }
        
        fclose(version_file);
}

/*
 * @brief Gets the system boot time
 */
void get_boot_time() {
        FILE *uptime_file;
        double uptime_seconds;
        time_t current_time, boot_time;
        struct tm *boot_tm;
        char boot_time_str[50];
        
        printf("\n3. System Boot Time:\n");
        printf("------------------\n");
        
        uptime_file = fopen("/proc/uptime", "r");
        if (uptime_file == NULL) {
                perror("Error opening /proc/uptime");
                return;
        }
        
        if (fscanf(uptime_file, "%lf", &uptime_seconds) != 1) {
                perror("Error reading uptime");
                fclose(uptime_file);
                return;
        }
        
        fclose(uptime_file);
        
        time(&current_time);
        boot_time = current_time - (time_t)uptime_seconds;
        boot_tm = localtime(&boot_time);
        
        strftime(boot_time_str, sizeof(boot_time_str), "%d:%H:%M:%S", boot_tm);
        printf("    System was last booted at: %s (day:hr:min:sec)\n", boot_time_str);
}

/*
 * @brief Gets the average system load
 */
void get_load_average() {
        FILE *loadavg_file;
        float load1, load5, load15;
        
        printf("\n4. System Load Average:\n");
        printf("--------------------\n");
        
        loadavg_file = fopen("/proc/loadavg", "r");
        if (loadavg_file == NULL) {
                perror("Error opening /proc/loadavg");
                return;
        }
        
        if (fscanf(loadavg_file, "%f %f %f", &load1, &load5, &load15) != 3) {
                perror("Error reading load average");
                fclose(loadavg_file);
                return;
        }
        
        fclose(loadavg_file);
        
        printf("    Load average (1 min): %.2f\n", load1);
        printf("    Load average (5 min): %.2f\n", load5);
        printf("    Load average (15 min): %.2f\n", load15);
}

/*
 * @brief Gets the system memory information
 */
void get_memory_info() {
        FILE *meminfo_file;
        char line[LINE_SIZE];
        unsigned long total_mem = 0, free_mem = 0, total_swap = 0, used_swap = 0, free_swap = 0;
        
        printf("\n5. Memory Information:\n");
        printf("-------------------\n");
        
        meminfo_file = fopen("/proc/meminfo", "r");
        if (meminfo_file == NULL) {
                perror("Error opening /proc/meminfo");
                return;
        }
        
        while (fgets(line, sizeof(line), meminfo_file) != NULL) {
                if (strncmp(line, "MemTotal:", 9) == 0) {
                        sscanf(line + 9, "%lu", &total_mem);
                } else if (strncmp(line, "MemFree:", 8) == 0) {
                        sscanf(line + 8, "%lu", &free_mem);
                } else if (strncmp(line, "SwapTotal:", 10) == 0) {
                        sscanf(line + 10, "%lu", &total_swap);
                } else if (strncmp(line, "SwapFree:", 9) == 0) {
                        sscanf(line + 9, "%lu", &free_swap);
                }
        }
        
        fclose(meminfo_file);
        
        used_swap = total_swap - free_swap;
        
        printf("    Total Memory: %lu KB (%.2f GB)\n", total_mem, total_mem / 1024.0 / 1024.0);
        printf("    Free Memory: %lu KB (%.2f GB)\n", free_mem, free_mem / 1024.0 / 1024.0);
        printf("    Used Memory: %lu KB (%.2f GB)\n", total_mem - free_mem, (total_mem - free_mem) / 1024.0 / 1024.0);
        
        printf("\n6. Swap Information:\n");
        printf("------------------\n");
        printf("    Total Swap: %lu KB (%.2f GB)\n", total_swap, total_swap / 1024.0 / 1024.0);
        printf("    Used Swap: %lu KB (%.2f GB)\n", used_swap, used_swap / 1024.0 / 1024.0);
}

/*
 * @brief Gets information about swap partitions
 */
void get_swap_partitions() {
        FILE *swaps_file;
        char line[LINE_SIZE];
        int line_num = 0;
        
        printf("\n7. Swap Partitions:\n");
        printf("-----------------\n");
        
        swaps_file = fopen("/proc/swaps", "r");
        if (swaps_file == NULL) {
                perror("Error opening /proc/swaps");
                return;
        }
        
        // Skip header line
        fgets(line, sizeof(line), swaps_file);
        
        while (fgets(line, sizeof(line), swaps_file) != NULL) {
                char name[100];
                char type[20];
                unsigned long size = 0;
                
                if (sscanf(line, "%s %s %lu", name, type, &size) >= 3) {
                        printf("    Swap #%d: %s (Type: %s, Size: %lu KB / %.2f GB)\n", 
                               ++line_num, name, type, size, size / 1024.0 / 1024.0);
                }
        }
        
        if (line_num == 0) {
                printf("    No swap partitions found\n");
        }
        
        fclose(swaps_file);
}

/*
 * @brief Gets CPU time statistics
 */
void get_cpu_stats() {
        FILE *stat_file;
        char line[LINE_SIZE];
        unsigned long user = 0, nice = 0, system = 0;
        
        printf("\n8. CPU Time Statistics:\n");
        printf("---------------------\n");
        
        stat_file = fopen("/proc/stat", "r");
        if (stat_file == NULL) {
                perror("Error opening /proc/stat");
                return;
        }
        
        if (fgets(line, sizeof(line), stat_file) != NULL) {
                // Format: cpu  user nice system idle iowait irq softirq steal guest guest_nice
                sscanf(line, "cpu %lu %lu %lu", &user, &nice, &system);
                
                printf("    CPU time in user mode: %lu jiffies\n", user);
                printf("    CPU time in user mode (nice): %lu jiffies\n", nice);
                printf("    CPU time in kernel mode: %lu jiffies\n", system);
                printf("    Total CPU time in user mode: %lu jiffies\n", user + nice);
                printf("    Note: 1 jiffy typically equals 0.01 seconds\n");
        }
        
        fclose(stat_file);
}

/*
 * @brief Gets context switch and interrupt information
 */
void get_context_switches_and_interrupts() {
        FILE *stat_file;
        char line[LINE_SIZE];
        unsigned long context_switches = 0;
        unsigned long interrupts = 0;
        
        printf("\n9. Context Switches:\n");
        printf("-----------------\n");
        
        stat_file = fopen("/proc/stat", "r");
        if (stat_file == NULL) {
                perror("Error opening /proc/stat");
                return;
        }
        
        while (fgets(line, sizeof(line), stat_file) != NULL) {
                if (strncmp(line, "ctxt ", 5) == 0) {
                        sscanf(line + 5, "%lu", &context_switches);
                        printf("    Total context switches: %lu\n", context_switches);
                }
                
                if (strncmp(line, "intr ", 5) == 0) {
                        sscanf(line + 5, "%lu", &interrupts);
                        printf("\n10. Interrupt Handling:\n");
                        printf("--------------------\n");
                        printf("    Total interrupts handled: %lu\n", interrupts);
                        break;
                }
        }
        
        fclose(stat_file);
}

/*
 * @brief Display system information using the /proc filesystem
 */
void system_info() {
        printf("\n=== SYSTEM INFORMATION (/proc filesystem) ===\n");
        
        get_cpu_info();
        get_kernel_version();
        get_boot_time();
        get_load_average();
        get_memory_info();
        get_swap_partitions();
        get_cpu_stats();
        get_context_switches_and_interrupts();
        
        printf("\n");
}

/*
 * @brief Gets process command line
 * 
 * @param pid Process ID
 */
void get_process_cmdline(int pid) {
        FILE *cmdline_file;
        char cmdline[BUFFER_SIZE];
        char filename[64];
        int i, len;
        
        printf("\n1. Process Command Line:\n");
        printf("---------------------\n");
        
        snprintf(filename, sizeof(filename), "/proc/%d/cmdline", pid);
        cmdline_file = fopen(filename, "r");
        
        if (cmdline_file == NULL) {
                perror("Error opening cmdline file");
                return;
        }
        
        len = fread(cmdline, 1, sizeof(cmdline) - 1, cmdline_file);
        fclose(cmdline_file);
        
        if (len <= 0) {
                printf("    No command line available\n");
                return;
        }
        
        cmdline[len] = '\0';
        
        // Convert null characters to spaces for display
        printf("    ");
        for (i = 0; i < len; i++) {
                if (cmdline[i] == '\0') {
                        printf(" ");
                } else {
                        printf("%c", cmdline[i]);
                }
        }
        printf("\n");
}

/*
 * @brief Gets process time statistics
 * 
 * @param pid Process ID
 */
void get_process_time(int pid) {
        FILE *stat_file;
        char filename[64];
        char buffer[BUFFER_SIZE];
        char state;
        unsigned long utime, stime;
        long cutime, cstime;
        unsigned long start_time;
        unsigned long hertz = sysconf(_SC_CLK_TCK);
        unsigned long uptime = 0;
        FILE *uptime_file;
        double uptime_sec;
        
        printf("\n2. Process Time Statistics:\n");
        printf("------------------------\n");
        
        // Get system uptime
        uptime_file = fopen("/proc/uptime", "r");
        if (uptime_file != NULL) {
                fscanf(uptime_file, "%lf", &uptime_sec);
                fclose(uptime_file);
                uptime = (unsigned long)uptime_sec;
        }
        
        snprintf(filename, sizeof(filename), "/proc/%d/stat", pid);
        stat_file = fopen(filename, "r");
        
        if (stat_file == NULL) {
                perror("Error opening process stat file");
                return;
        }
        
        if (fgets(buffer, sizeof(buffer), stat_file) != NULL) {
                // Format: pid (comm) state ppid ... utime stime ...
                char *comm_start = strchr(buffer, '(');
                char *comm_end = strrchr(buffer, ')');
                
                if (comm_start && comm_end && comm_end > comm_start) {
                        char temp = *comm_end;
                        *comm_end = '\0';
                        
                        // Create a new string with everything after the comm field
                        char rest_of_line[BUFFER_SIZE];
                        strncpy(rest_of_line, comm_end + 1, sizeof(rest_of_line));
                        rest_of_line[sizeof(rest_of_line) - 1] = '\0';
                        
                        // Parse the remaining fields
                        sscanf(rest_of_line, " %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %ld %ld %*ld %*ld %*ld %*ld %lu",
                               &state, &utime, &stime, &cutime, &cstime, &start_time);
                               
                        *comm_end = temp;
                        
                        // Calculate run time
                        unsigned long seconds_since_boot = start_time / hertz;
                        unsigned long process_runtime = uptime - seconds_since_boot;
                        
                        printf("    Process state: %c\n", state);
                        printf("    Time running: %lu seconds (%.2f hours)\n", 
                               process_runtime, process_runtime / 3600.0);
                        printf("    Time spent in user mode: %lu jiffies (%.2f seconds)\n", 
                               utime, utime / (double)hertz);
                        printf("    Time spent in kernel mode: %lu jiffies (%.2f seconds)\n", 
                               stime, stime / (double)hertz);
                        printf("    Time spent in user mode (children): %ld jiffies (%.2f seconds)\n", 
                               cutime, cutime / (double)hertz);
                        printf("    Time spent in kernel mode (children): %ld jiffies (%.2f seconds)\n", 
                               cstime, cstime / (double)hertz);
                }
        }
        
        fclose(stat_file);
}

/*
 * @brief Gets process environment variables
 * 
 * @param pid Process ID
 */
void get_process_environ(int pid) {
        FILE *environ_file;
        char filename[64];
        char env_buffer[MAX_ENV_SIZE];
        int i, len;
        
        printf("\n4. Process Environment:\n");
        printf("--------------------\n");
        
        snprintf(filename, sizeof(filename), "/proc/%d/environ", pid);
        environ_file = fopen(filename, "r");
        
        if (environ_file == NULL) {
                perror("Error opening process environ file");
                return;
        }
        
        len = fread(env_buffer, 1, sizeof(env_buffer) - 1, environ_file);
        fclose(environ_file);
        
        if (len <= 0) {
                printf("    No environment variables available\n");
                return;
        }
        
        env_buffer[len] = '\0';
        
        // Print each environment variable
        printf("    Environment variables:\n");
        for (i = 0; i < len; i++) {
                if (env_buffer[i] == '\0') {
                        printf("\n    ");
                } else {
                        printf("%c", env_buffer[i]);
                }
        }
        printf("\n");
}

/*
 * @brief Gets process memory map
 * 
 * @param pid Process ID
 */
void get_process_maps(int pid) {
        FILE *maps_file;
        char filename[64];
        char line[LINE_SIZE];
        int count = 0;
        
        printf("\n5. Process Address Space Map:\n");
        printf("--------------------------\n");
        
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
        maps_file = fopen(filename, "r");
        
        if (maps_file == NULL) {
                perror("Error opening process maps file");
                return;
        }
        
        printf("    Memory map segments (first 10 shown):\n");
        printf("    %-12s %-8s %-4s %-8s %-8s %s\n", 
               "Address", "Perms", "Dev", "Offset", "Inode", "Pathname");
               
        while (fgets(line, sizeof(line), maps_file) != NULL && count < 10) {
                printf("    %s", line);
                count++;
        }
        
        if (!feof(maps_file)) {
                printf("    ... (additional segments not shown)\n");
        }
        
        fclose(maps_file);
}

/*
 * @brief Display detailed information about a specific process
 * 
 * @param pid Process ID
 */
void process_info(int pid) {
        char proc_dir[64];
        struct stat st;
        
        // Check if the process exists
        snprintf(proc_dir, sizeof(proc_dir), "/proc/%d", pid);
        if (stat(proc_dir, &st) != 0) {
                printf("Error: Process with PID %d does not exist\n", pid);
                return;
        }
        
        printf("\n=== PROCESS INFORMATION (PID: %d) ===\n", pid);
        
        get_process_cmdline(pid);
        get_process_time(pid);
        // Process user/kernel mode time is already covered in process_time
        get_process_environ(pid);
        get_process_maps(pid);
        
        printf("\n");
}

/*
 * @brief Main handler for task 9
 * 
 * Explores the /proc filesystem to gather system and process information
 */
void run_task9() {
        extern uint8_t argc;
        extern char *argv[];
        int pid = -1;
        
        printf("\n=== TASK 9: Explore /proc filesystem ===\n\n");
        printf("Usage options:\n");
        printf("1. ./a.out system      - Show system information\n");
        printf("2. ./a.out process <pid> - Show process information for the specified PID\n");
        printf("3. ./a.out both <pid>    - Show both system and process information\n\n");
        
        get_cmd_arg();
        
        if (argc < 2) {
                printf("No option specified. Showing system information by default.\n");
                system_info();
        } else if (strcmp(argv[1], "system") == 0) {
                system_info();
        } else if (strcmp(argv[1], "process") == 0) {
                if (argc < 3) {
                        printf("Error: Process PID not specified\n");
                        printf("Usage: ./a.out process <pid>\n");
                } else {
                        pid = atoi(argv[2]);
                        if (pid <= 0) {
                                printf("Error: Invalid PID '%s'\n", argv[2]);
                        } else {
                                process_info(pid);
                        }
                }
        } else if (strcmp(argv[1], "both") == 0) {
                if (argc < 3) {
                        printf("Error: Process PID not specified\n");
                        printf("Usage: ./a.out both <pid>\n");
                } else {
                        pid = atoi(argv[2]);
                        if (pid <= 0) {
                                printf("Error: Invalid PID '%s'\n", argv[2]);
                                system_info();
                        } else {
                                system_info();
                                process_info(pid);
                        }
                }
        } else {
                printf("Unknown option: %s\n", argv[1]);
                printf("Usage options:\n");
                printf("1. ./a.out system      - Show system information\n");
                printf("2. ./a.out process <pid> - Show process information for the specified PID\n");
                printf("3. ./a.out both <pid>    - Show both system and process information\n");
        }
        
        free_arg();
        
        printf("Press Enter to continue...");
        getchar();
}
