/*
 * File Name: questions.h
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */


#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <stdint.h>


/*
 * @brief Task 1: Create a file with a 4KB hole (sparse file).
 *
 * This function demonstrates how to use `lseek()` to create a sparse file
 * that appears to have a large size but doesn't actually allocate disk blocks
 * for empty regions.
 */
void run_task1();


/*
 * @brief Task 2: Copy the contents of one file into another.
 *
 * This function reads data from a source file and writes it into a destination file.
 * It typically uses low-level file I/O system calls such as `open()`, `read()`, and `write()`.
 */
void run_task2();


/*
 * @brief Task 3: Display the contents of a directory with file type annotations.
 *
 * This function lists files in a given directory and displays the type of each file
 * (regular file, directory, symbolic link, etc.).
 */
void run_task3();


/*
 * @brief Task 4: Fix and test code by compiling it with proper flags.
 *
 * This task involves analyzing broken or warning-prone code,
 * compiling it with `gcc` flags (e.g., `-Wall`, `-Werror`), and fixing the issues.
 */
void run_task4();


/*
 * @brief Task 5: Test and analyze the given code snippet.
 *
 * This function runs a provided piece of code, inspects its behavior,
 * and may involve debugging, profiling, or explanation of output.
 */
void run_task5();


/*
 * @brief Task 6: Reimplement and explore variants of the `cat` command.
 *
 * This function replicates the functionality of the Unix `cat` command,
 * and explores its behavior under different scenarios (e.g., reading from stdin, multiple files).
 */
void run_task6();


/*
 * @brief Task 7: Answer questions about standard I/O buffering and implement `cat` using stdio.
 *
 * This task emphasizes the use of `fopen()`, `fgets()`, and other high-level standard I/O functions.
 * It may also explore buffering modes (`_IOFBF`, `_IOLBF`, `_IONBF`).
 */
void run_task7();


/*
 * @brief Task 8: Implement simplified versions of the `ls` command.
 *
 * This function emulates `ls` by reading directory contents and optionally adding support for flags like `-l`, `-a`.
 */
void run_task8();


/*
 * @brief Task 9: Explore and manipulate entries in the `/proc` filesystem.
 *
 * This function accesses virtual files under `/proc` (e.g., `/proc/self`, `/proc/[pid]/status`)
 * to demonstrate how Linux exposes process and system info.
 */
void run_task9();


#endif // QUESTIONS_H

