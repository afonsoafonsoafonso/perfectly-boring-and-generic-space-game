#pragma once

#include "game_structs.h"
#include "sllist.h"

/** @defgroup player player
 * @{
 * 
 * 
 *  Functions responsible for player's functionality (moving, shooting, dying, collisions etc)
 */

#define COL_LEFT 2 
#define COL_RIGHT 1
#define NO_COL 0 

//macros for checking out of bounds
#define IN_BOUNDS 0
#define OFF_BOUNDS_R 1
#define OFF_BOUNDS_L 2

//macros for player keyboard movement
#define KBC_INT 0
#define TIMER_INT 1
#define ON_GOING_ANIMATION 1
#define MOVE_U 1
#define MOVE_R 2
#define MOVE_D 3
#define MOVE_L 4

/**
 * @brief Makes player spaceship move accordingly to keyboard input
 * 
 * @param sp Player spaceship's sprite
 * @param scancode Keyboard scancode read from the output buffer
 * @param game_data Struct containing game info, sprites and variables including movement animation and direction flags and movement animation frame counter
 * @param kbc_mouse_int Flag specifying if function is being used in a keyboard or mouse interrupt
 */
void player_move_kbc(Sprite *sp, uint8_t *scancode, struct game_data* game_data, int kbc_mouse_int);

/**
 * @brief Makes player spaceship move accordingly to mouse input
 * 
 * @param sp Player spaceship's sprite
 * @param pp Mouse three byte packet containing input info
 */
void player_move_mouse(Sprite *sp, struct packet *pp);

/**
 * @brief Function responsible for making player explosion animation when hit by enemy bullet
 * 
 * @param game_data Struct containing game info, sprites and variables including all explosion frames, flag and coordenates
 */
void player_explosion(struct game_data* game_data);

/**
 * @brief Function responsible for making player bullet move each frame and checking if the bullet hit an enemy using CheckBuleltCollision()
 * 
 * @param game_data Struct containing game info, sprites and variables including player score, bullet sprite and flags
 * @return int Returns 0 if bullet movement was not impeded and 1 otherwise (either by going out of bounds or hitting an enemy)
 */
int player_bullet_move(struct game_data* game_data);

/**
 * @brief Checks if player bullet has hit an enemy and, if so, removes it from the linked list
 * 
 * @param game_data Struct containing game info, sprites and variables
 * @param bullet Player bullet's sprite
 * @param enemies Linked-list containing all enemy spaceships sprites
 * @return int 
 */
int CheckBulletCollision(struct game_data* game_data, Sprite *bullet, struct sllist *enemies);



