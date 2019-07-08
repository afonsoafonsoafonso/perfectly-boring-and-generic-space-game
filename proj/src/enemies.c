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
#include "game_states.h"   
#include "video_card.h"
#include "rtc.h"
#include "enemies.h"
#include "cursor.h"
#include "hud.h"
#include "utils.h"

void enemy_move(struct sllist *ll) {
    struct lnode *node=ll->head;
    Sprite* current = NULL;
    int bounds_flag=0;

    for(int i=0; i<ll->size; i++) {
        current=(Sprite*)(node->data);
        erase_drawn_sprite(current);
        ship_col_handler(current,ll,i);
        bounds_flag=CheckMoveXBoundaries(current);
        if(bounds_flag==OFF_BOUNDS_L) {
            current->xspeed = 0 - current->xspeed;
        }
        else if(bounds_flag==OFF_BOUNDS_R) {
            current->xspeed = 0 - current->xspeed;
        }
        current->x = current->x + current->xspeed;

        draw_sprite(current,current->x,current->y);
        node->data=current;
        node=node->next;
    }
    return;
}

void enemy_explosion(struct game_data* game_data) {
    fill_exp_array_metadata(game_data,game_data->exp_x,game_data->exp_y);
    if( (game_data->exp_frame_counter < 8) && game_data->exp_flag) {
        if(game_data->exp_frame_counter==0) {
            draw_sprite(game_data->sprite->explosion[0], game_data->exp_x, game_data->exp_y);
        }
        else {
            erase_drawn_sprite(game_data->sprite->explosion[game_data->exp_frame_counter-1]);
            draw_sprite(game_data->sprite->explosion[game_data->exp_frame_counter], game_data->exp_x, game_data->exp_y);
        }
        (game_data->exp_frame_counter)++;
    }
    else {
        erase_drawn_sprite(game_data->sprite->explosion[8]);
        game_data->exp_x=-15;
        game_data->exp_y=-15;
        game_data->exp_frame_counter=0;
        game_data->exp_flag=0;
    }
    return;
}

//returns 1 if hit, 0 otherwise
int CheckEnemyBulletHit(struct game_data* game_data) {
    int bx, by, px, py;
    px = game_data->sprite->player->x;
    py = game_data->sprite->player->y;
    for(int i=0; i<3; i++) {
        bx=game_data->sprite->enemy_bullet[i]->x;
        by=game_data->sprite->enemy_bullet[i]->y;
            if( (by+20 >= py && by+20 <= py+80) && (bx >= px && bx+15 <= px+80) && game_data->invulnerability_flag==0) {
                erase_drawn_sprite(game_data->sprite->player);
                erase_drawn_sprite(game_data->sprite->enemy_bullet[i]);
                game_data->enemy_bullet_flag[i]=0;
                return 1;
            }   
    }
    return 0;
}

void enemy_fire(struct game_data* game_data) {
    struct lnode* current_node = NULL;
    Sprite *sp=NULL;
    int r;
    for(int i=0; i<3; i++) {
        current_node=game_data->sprite->enemies->head;
        if(game_data->enemy_bullet_flag[i]==0) {
            r = rand()%game_data->sprite->enemies->size;
            for(int j=0; j<r; j++) current_node=current_node->next;;
            sp=(Sprite*)(current_node->data);
            game_data->sprite->enemy_bullet[i]->x=(sp->x)+40;
            game_data->sprite->enemy_bullet[i]->y=(sp->y)+80;
            draw_sprite(game_data->sprite->enemy_bullet[i],game_data->sprite->enemy_bullet[i]->x,game_data->sprite->enemy_bullet[i]->y);
            game_data->enemy_bullet_flag[i]=1;
        }
    }
    return;
}


int enemy_bullet_movement(struct game_data* game_data) {
    for(int i=0; i < 3; i++) {
        erase_drawn_sprite(game_data->sprite->enemy_bullet[i]);
        game_data->sprite->enemy_bullet[i]->y += game_data->sprite->enemy_bullet[i]->yspeed;
        if(game_data->sprite->enemy_bullet[i]->y > (768-20)) {
            game_data->enemy_bullet_flag[i]=0;
        }
        draw_sprite(game_data->sprite->enemy_bullet[i],game_data->sprite->enemy_bullet[i]->x,game_data->sprite->enemy_bullet[i]->y);
        if(CheckEnemyBulletHit(game_data)==1) {
            return 1;
        }
    }
    return 0;
}

void ship_col_handler(Sprite*ship, struct sllist* ll, int ll_pos) {
    struct lnode *node=ll->head;
    Sprite*current = NULL;
    int next_pos = ship->x + ship->xspeed;

    for(int i=0; i<ll_pos+1; i++) {
        if(i==ll->size) break;

        current = (Sprite*)(node->data);
        if(i==ll_pos-1) {
            if(next_pos <= current->x+80 && ship->y == current->y) {
                make_negative(&current->xspeed);
                make_positive(&ship->xspeed);
            }
        }
        else if(i==ll_pos+1) {
            if(next_pos >= current->x && ship->y == current->y) {
                make_negative(&ship->xspeed);
                make_positive(&current->xspeed);
            }
        }
        node->data=current;
        node=node->next;
    }

    return;
}

