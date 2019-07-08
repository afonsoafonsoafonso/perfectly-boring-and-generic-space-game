#pragma once

/** @defgroup utils utils
 * @{
 * 
 * 
 *  Functions of general utility
 */

/**
 * @brief Turns any given number into a negative one
 * 
 * @param x Pointer to integer to be turned into negative
 */
void make_negative(int* x);

/**
 * @brief Turns any given number into a positive one
 * 
 * @param x Pointer to integer to be turned into positive 
 */
void make_positive(int* x);

/**
 * @brief Turns any given integer number into a string
 * 
 * @param number Integer to be turned into string
 * @return char* String with integer 
 */
char* int_to_array(int number);


/**
 * @brief Checks if given sprite will go out of bounds in the X axis in it's next movement
 * 
 * @param sp Sprite whose movement will be checked
 * @return int Returns 0 if it will remain in bounds and 1 if it will go out of bounds
 */
int CheckMoveXBoundaries(Sprite *sp);

/**
 * @brief Checks if given sprite will go out of bounds in the y axis in it's next movement
 * 
 * @param sp Sprite whose movement will be checked
 * @return int Returns 0 if it will remain in bounds and 1 if it will go out of bounds
 */
int CheckMoveYBoundaries(Sprite* sp);

/**
 * @brief Fills all explosion frames' sprites data with given x and y values
 * 
 * @param game_data Struct containing explosion frames
 * @param x X coordenate of the explosion
 * @param y Y coordenate of the explosion
 */
void fill_exp_array_metadata(struct game_data* game_data, int x, int y);

