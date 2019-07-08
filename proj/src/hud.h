#pragma once

/** @defgroup hud hud
 * @{
 * 
 * 
 * 
 *  Functions responsible for the game's hud: score, lives and win and loss screen
 */
  
/**
 * @brief Draws current player no of lives in bottom right corner of screen
 * 
 * @param game_data Struct containing game info including player lives
 */
void draw_player_lives(struct game_data* game_data);

/**
 * @brief Draws sprite notifying user if they won or lost the game
 * 
 * @param game_data Struct containing game info including game won/game lost sprites
 */
void print_endgame_screen(struct game_data* game_data);

/**
 * @brief Erases previously drawn player score in last frame
 * 
 */
void erase_drawn_score();

void erase_drawn_date();

/**
 * @brief Function that displays given number (n) in given coordenates
 * 
 * @param game_data Struct containing game info including sprites for each number and character possibly needed
 * @param n Number to be displayed on screen
 * @param x Adress of variable containing coordenate where number will be displayed
 * @param y Y coordenate where number will be displayed
 */
void display_number(struct game_data* game_data, int n, int *x, int y);

/**
 * @brief Prints current player score at bottom left corner of the screen
 * 
 * @param game_data Struct containing game info including current player score
 */
void print_score(struct game_data *game_data);

/**
 * @brief Displays current date by polling the rtc
 * 
 * @param game_data Struct containing game info including sprites of numbers and characters needed
 */
void display_date(struct game_data* game_data);

