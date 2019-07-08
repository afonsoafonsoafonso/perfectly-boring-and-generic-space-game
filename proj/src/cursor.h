#pragma once

/** @defgroup cursor cursor
 * @{
 * 
 * 
 *  Functions responsible for cursor
 */

/**
 * @brief Responsible for making the mouse move. Erases previous mouse sprite and draws it again considering mouse input from user (reading it from struct packet).
 * 
 * @param cursor Sprite containing cursor xpm image
 * @param pp Struct storing mouse input information
 */
void cursor_move(Sprite *cursor, struct packet *pp);

/**
 * @brief Checks if any button from the menu, Play or Exit, has been pressed and handles it accordingly.
 * 
 * @param pp Struct storing mouse input information
 * @param cursor Sprite containing cursor xpm image
 * @param state Game current state
 * @return int Returns 1 if Play/Exit were pressed, 0 otherwise
 */
int check_button_click(struct packet* pp, Sprite* cursor, int *state);


