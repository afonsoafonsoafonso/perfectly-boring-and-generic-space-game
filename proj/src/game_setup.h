#pragma once
#include "game_structs.h"

/** @defgroup game_setup game_setup
 * @{
 * 
 * 
 *  Functions responsible for setting up and initializing the game 
 */

/**
 * @brief Subscribes and initializes all devices (timer,keyboard,mouse,rtc)
 * 
 * @param io_data Struct containing devices data
 * @return int Returns 1 if there was a problem, 0 otherwise
 */
int ints_init(struct devices_data *io_data);

/**
 * @brief Initializes all sprites needed for the game
 * 
 * @param background Sprite global variable containing the background
 * @param sprite Struct containing sprites used in game
 */
int sprites_init(Sprite **background, struct sprites *sprite);

/**
 * @brief Initializes background by drawing background menu image and the cursor
 * 
 * @param background Sprite global variable containing the backgroud
 * @param cursor Sprite containing user's cursor
 */
void menu_init(Sprite *background, Sprite* cursor);

/**
 * @brief Initializes all flags, variables and also uses sprites_init() and menu_ini() to initialize the game as a whole
 * 
 * @param background Sprite global variable containing the backgroud
 * @param game_data Struct containing game variables, sprites, flags and other data
 * @param io_data Struct containing devices data
 */
void game_init(Sprite **background, struct game_data *game_data, struct devices_data *io_data);

/**
 * @brief Deletes all game sprites and clears it's memory
 * 
 * @param background Sprite global variable containing the backgroud
 * @param sprite Struct containing all game sprites
 */
void delete_all_sprites(Sprite **background, struct sprites *sprite);

/**
 * @brief Unsubcribes all devices and resets them
 * 
 * @return int Returns -1 if something went wrong, 0 otherwise
 */
int ints_unsub();

/**
 * @brief Uses previous functions to correctly exit the game
 * 
 * @param background Sprite containing background
 * @param sprite Struct containing all game sprites
 */
void game_exit(Sprite **background, struct sprites *sprite);
