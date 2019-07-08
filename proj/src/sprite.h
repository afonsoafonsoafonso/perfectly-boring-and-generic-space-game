#pragma once

/** @defgroup sprite sprite
 * @{
 * 
 * 
 * Sprite functions and structs made by Joao Cardoso (jcard@fe.up.pt) and added by pfs@fe.up.pt
 */

/**
 * @brief Struct containing all its metadata and the pixmap of the sprite
 * 
 */
typedef struct {
  int x;            /**< current sprite x position */
  int y;         /**< current sprite y position */
  int width;  /**< sprite width*/
  int height;   /**< sprite height */
  int xspeed, yspeed;  /**< current speeds in the x and y direction */
  uint16_t *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/**
 * @brief Global variable containing the background sprite
 * 
 */
Sprite* background;

/**
 * @brief Create a sprite object
 * 
 * @param pic Pixmap with sprite itself (the image)
 * @param x X coordenate
 * @param y Y coordenate
 * @param xspeed Speed in the X axis
 * @param yspeed Speed in the Y axis
 * @return Sprite* Returns Sprite struct with all it's data
 */
Sprite * create_sprite(xpm_string_t* pic, int x, int y, int xspeed, int yspeed);

/**
 * @brief Draws the given sprite at the given coordenates
 * 
 * @param sp Sprite object
 * @param x Sprite X coordenate
 * @param y Sprite Y coordenate
 */
void draw_sprite(Sprite* sp, int x, int y);

/**
 * @brief Destroys sprite object and frees all it's memory
 * 
 * @param sp Sprite to be destroyed
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Erases previously drawn Sprite accordingly to it's metadata
 * 
 * @param sp Sprite to be erases
 */
void erase_drawn_sprite(Sprite *sp);



