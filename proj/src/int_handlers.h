#pragma once

#include "game_structs.h"
#include "sprite.h"
#include "sllist.h"
#include "game_states.h"

/** @defgroup int_handlers int_handlers
 * @{
 * 
 * 
 * 
 *  Functions responsible for handling interrupts using all other functions
 */

/**
 * @brief Handles keyboard interrupts
 * 
 * @param scancode 
 * @param game_data 
 */
void keyboard_handler(uint8_t* scancode, struct game_data *game_data);

/**
 * @brief Handles mouses interrupts
 * 
 * @param bytecounter Counts mouse packets already receives
 * @param mouse_data Mouse data received from output buffer
 * @param pp Struct storing mouse data after being parsed
 * @param cursor Cursor sprite
 * @param player Player sprite
 */
void mouse_handler(int* bytecounter, uint32_t mouse_data, struct packet* pp, Sprite* cursor, Sprite* player, struct game_data* game_data);

/**
 * @brief Handles timer interrupts
 * 
 * @param state Current game state
 * @param scancode Last keyboard scancode
 * @param io_data Struct containing devices data
 * @param game_data Struct containing game data, variables, sprites and info
 */
void timer_handler(int *state, uint8_t *scancode,struct devices_data *io_data, struct game_data *game_data);


