#pragma once

/** @defgroup enemies enemies
 * @{
 * 
 * 
 * 
 *  Functions responsible for enemies and it's actions during the game
 */

/**
 * @brief Responsible for handling enemy spaceships movement
 * 
 * @param ll Linked-list containing all enemy-spaceships sprites
 */
void enemy_move(struct sllist *ll);


/**
 * @brief Makes enemy explosion animation
 * 
 * @param game_data Struct containing most game variables and information including all explosion animation frames, explosion intended x and y coordenates, explosion frame counter and explosion flag (set when an enemy explosion is intended to happen)
 */
void enemy_explosion(struct game_data* game_data);

/**
 * @brief Checks if enemy bullet has hit player spaceship or not
 * 
 * @param game_data Struct containing most game variables and data including player spaceship sprite and an array containing all bullets sprite's
 * @return int Returns 1 if enemy bullet has hit the player and 0 otherwise
 */
int CheckEnemyBulletHit(struct game_data* game_data);

/**
 * @brief Responsible for making three randomly chosen enemies fire.
 * 
 * @param game_data Struct containing most game variables and data including the linked-list with all enemies sprites and an array with enemy bullet flags which are set when it's respective bullet is fired (one flag for each possible bullet)
 */
void enemy_fire(struct game_data* game_data);

/**
 * @brief Responsible for making each bullet move each frame. Also uses CheckEnemyBulletHit() function to check if any of the bullets hit the player
 * 
 * @param game_data Struct containing most game variables and data including the array with all bullet sprites and it's flags
 * @return int Returns 1 if bullet hit player's spaceship, 1 otherwise
 */
int enemy_bullet_movement(struct game_data* game_data);

/**
 * @brief Analyzes given ship movement in order to check if it will collide with any other ship. If so, it makes them go in opposite directions instead
 * 
 * @param ship Ship whose movement will be checked for any possible collisions
 * @param ll Linked-list containing all enemy spaceships
 * @param ll_pos Ship whose movement will be analyzed in the linked-list that contains itself and all other enemy spaceships
 */
void ship_col_handler(Sprite*ship, struct sllist* ll, int ll_pos);


