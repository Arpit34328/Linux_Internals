/*
 * File Name: cmd_arg.h
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */


#ifndef CMD_ARG_H
#define CMD_ARG_H


/*
 * @brief Reads command-line style input from the user and parses it into argv/argc style.
 *        Only supports up to 5 arguments (including "./a.out").
 */
void get_cmd_arg();


/*
 * @brief Frees dynamically allocated memory used for storing command-line arguments.
 */
void free_arg();

#endif // CMD_ARG_H
