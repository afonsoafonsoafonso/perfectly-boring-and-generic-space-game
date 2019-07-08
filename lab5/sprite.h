/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

typedef struct {
  int x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  int xspeed, yspeed;  /**< current speeds in the x and y direction */
  char *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

Sprite * create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed);

void draw_sprite(Sprite* sp, int x, int y);

void destroy_sprite(Sprite *sp);

void erase_drawn_sprite(Sprite *sp);

int move_sprite(Sprite *sp, int xf, int xy);

#endif
