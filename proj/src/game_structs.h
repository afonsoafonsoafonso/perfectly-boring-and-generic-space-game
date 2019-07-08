#pragma once
#include "sprite.h"

/** @defgroup game_structs game_structs
 * @{
 * 
 * 
 * 
 *  Structs storing i/o and game data, including all sprites (except background)
 */


/**
 * @brief Struct containing all game sprites (except background)
 * 
 */
struct sprites {
  Sprite* player; //!< Player spaceship sprite
  Sprite* bullet; //!< Player bullet sprite
  Sprite* cursor; //!< Cursor sprite
  Sprite* enemy_bullet[3]; //!< Array of enemy bullet sprites
  Sprite* explosion[9]; //!< Array with enemy explosion frames' sprites
  Sprite* player_exp[9]; //!< Array with player explosion frames' sprites
  Sprite* hearts[3]; //!< Array with hearts sprites
  Sprite* gamewon; //!< Gamewon screen sprite
  Sprite* gamelost; //!< Gamelost screen sprite
  Sprite* score; //!< Sprite contaning "Score:"
  Sprite* number[10]; //!< Array of numbers' sprites
  Sprite* slash; //!< '/' sprite
  Sprite* colon; //!< ':' sprite
  struct sllist* enemies; //!< Linked list containing all enemies' sprites
};

/**
 * @brief Struct containing game data, sprites, info and variables
 * 
 */
struct game_data{
  int state; //!< Game state
  int score; //!< Player score
  int player_lives; //!< Player no of lives
  int invulnerability_flag; //!< Player invulnerability flag
  int player_exp_flag; //!< Player explosion flag (set if a explosion must/is happen/happenin)
  int player_exp_frame_cnt; //!< Player explosion frame counter

  struct sprites* sprite; //!< Struct contaning game sprites
  int bullet_flag; //!< Player bullet flag (set if bullet was fired and stays set until it hits something or goes out of bounds)
  int enemy_bullet_flag[4]; //!< Enemy bullet flag (set if bullet was fired and stays set until it hits something or goes out of bounds)

  int move_frame_counter; //!< Player move animation frame counter
  int move_direction_flag; //!< Player direction  flag (MOVE_U , MOVE_L , MOVE_D OR MOVE_R)
  int on_going_anim_flag; //!< Current animation flag (set if player move animation is on going)

  int exp_flag; //!< Set if there's an enemy explosion starting or currently happening
  int exp_frame_counter; //!< Enemy Explosion animation frame counter
  int exp_x; //!< Current enemy explosion x coordenate
  int exp_y; //!< Current enemy explosion y coordenate

  int player_exp_x; //!< Current Player explosion x coordenate
  int player_exp_y; //!< Current player explosion y coordeante
};

struct devices_data{
  uint8_t timer_bitmask; //!< Timer bitmask
  int timer_int_cnt; //!< Timer interrupts counter

  int keyboard_bitmask; //!< Keyboard bitmask

  int mouse_bitmask; //!< Mouse bitmask
  struct packet mouse_pp; //!< Struct containing mouse packet bytes and its info (already parsed)
  int byte_counter; //!< Mouse byte counters

  int rtc_bitmask; //!< Real time clock counter
};

