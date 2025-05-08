/*
 * File Name: p_set.h
 * Author: Arpit Patel
 * Created: 05 May, 2025
 */

#ifndef P_SET_H
#define P_SET_H

#include <stdint.h>


/*
 * @brief Displays the list of available questions to run.
 */
void display_question();


/*
 * @brief Prompts the user to select a question (1 to 5) and returns the choice.
 *        Handles invalid input and retries until valid selection is made.
 *
 * @return uint8_t The selected question number (1 to 5).
 */
uint8_t get_question();


/*
 * @brief Runs a dummy placeholder for the selected question number.
 *
 * @param q_no Question number (1 to 5).
 */
void run_question(uint8_t q_no);


#endif // P_SET_H

