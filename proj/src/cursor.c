// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "sllist.h"
#include "keyboard.h"
#include "sprite.h"
#include "player.h"
#include "../res/xpm.h"
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "game_setup.h"
#include "int_handlers.h"
#include "stdlib.h"
#include "math.h"
#include "rtc.h"

void cursor_move(Sprite *cursor, struct packet *pp) {
    erase_drawn_sprite(cursor);
    cursor->x+=pp->delta_x;
    cursor->y-=pp->delta_y;
    draw_sprite(cursor,cursor->x,cursor->y);
    return;
}

int check_button_click(struct packet* pp, Sprite* cursor, int *state) {
    if(pp->lb == true) {
        if( (cursor->x >= 437 && cursor->x <= 587)&&(cursor->y >= 300 && cursor->y <= 358)) {
            background=create_sprite(space_xpm,0,0,0,0);
            draw_sprite(background,0,0);
            *state = GAME;
            return 1;
        }
        if((cursor->x >=447 && cursor->x <= 577)&&(cursor->y >= 400 && cursor->y <= 458)) {
            *state = EXIT;
            return 1;
        }
    }
    return 0;
}


