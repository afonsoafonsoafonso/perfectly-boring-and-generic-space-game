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
#include "utils.h"

extern unsigned long handler_counter;
extern Sprite* background;

void player_move_kbc(Sprite *sp, uint8_t *scancode, struct game_data* game_data, int kbc_timer_int) {
    switch(kbc_timer_int) {
        case KBC_INT:
            game_data->invulnerability_flag=0;
            if(game_data->on_going_anim_flag) return;

            if(*scancode == 0x48) {
                game_data->move_direction_flag=MOVE_U;
                game_data->move_frame_counter=0;
            }
            else if(*scancode == 0x50) {
                game_data->move_direction_flag=MOVE_D;
                game_data->move_frame_counter=0;
            }
            else if(*scancode == 0x4d) {
                game_data->move_direction_flag=MOVE_R;
                game_data->move_frame_counter=0;
            }
            else if(*scancode == 0x4b) {
                game_data->move_direction_flag=MOVE_L;
                game_data->move_frame_counter=0;
            }
            else return;

            break;

        case TIMER_INT:
            if(game_data->move_direction_flag!=0) game_data->on_going_anim_flag=1;

            if(game_data->move_frame_counter<10) {

                switch(game_data->move_direction_flag) {
                    case MOVE_U:
                        if(sp->y >= 384){
                            erase_drawn_sprite(sp);
                            sp->y=sp->y-5;
                            draw_sprite(sp, sp->x, sp->y);
                        }
                        break;
                    case MOVE_D:
                        if(sp->y <= (768 -85)){
                            erase_drawn_sprite(sp);
                            sp->y=sp->y+5;
                            draw_sprite(sp, sp->x, sp->y);
                        }
                        break;
                    case MOVE_R:
                        if(sp->x <= (1024 - 85)){
                            erase_drawn_sprite(sp);
                            sp->x=sp->x+5;
                            draw_sprite(sp, sp->x, sp->y);
                        }
                        break;
                    case MOVE_L:
                        if(sp->x >= 3){
                            erase_drawn_sprite(sp);
                            sp->x=sp->x-5;
                            draw_sprite(sp, sp->x, sp->y);
                        }
                        break;
                }
                (game_data->move_frame_counter)++;

                return;
            }
            else {
                game_data->on_going_anim_flag=0;
                game_data->move_direction_flag=0;
                game_data->move_frame_counter=0;
                return;
            }

            break;

    }
}

void player_move_mouse(Sprite *sp, struct packet *pp) {
    erase_drawn_sprite(sp);
    sp->x+=(pp->delta_x/2);
    sp->y-=(pp->delta_y/2);
    draw_sprite(sp, sp->x,sp->y);
    return;
}

void player_explosion(struct game_data* game_data) {
    if( (game_data->player_exp_frame_cnt<8) && game_data->player_exp_flag) {
        if(game_data->player_exp_frame_cnt!=0) {
            erase_drawn_sprite(game_data->sprite->player_exp[game_data->player_exp_frame_cnt-1]);
        }
        game_data->sprite->player_exp[game_data->player_exp_frame_cnt]->x=game_data->player_exp_x;
        game_data->sprite->player_exp[game_data->player_exp_frame_cnt]->y=game_data->player_exp_y;
        draw_sprite(game_data->sprite->player_exp[game_data->player_exp_frame_cnt],game_data->sprite->player_exp[game_data->player_exp_frame_cnt]->x,game_data->sprite->player_exp[game_data->player_exp_frame_cnt]->y);
        (game_data->player_exp_frame_cnt)++;
    }
    else if (game_data->player_exp_frame_cnt==8 && game_data->player_exp_flag) {
        erase_drawn_sprite(game_data->sprite->player_exp[8]);
        game_data->player_exp_flag=0;
        game_data->player_exp_frame_cnt=0;
    }   
    return;
}

//returns 0 if movement not impeded, 1 otherwise
int player_bullet_move(struct game_data* game_data) {
    erase_drawn_sprite(game_data->sprite->bullet);
    if(CheckMoveYBoundaries(game_data->sprite->bullet)==0) {
        game_data->sprite->bullet->y = game_data->sprite->bullet->y + game_data->sprite->bullet->yspeed;
        draw_sprite(game_data->sprite->bullet,game_data->sprite->bullet->x,game_data->sprite->bullet->y);
        draw_sprite(game_data->sprite->player, game_data->sprite->player->x, game_data->sprite->player->y);
        if(CheckBulletCollision(game_data,game_data->sprite->bullet,game_data->sprite->enemies)==1) {
            game_data->exp_flag=1;
            game_data->exp_frame_counter=0;
            //game_data->exp_x=game_data->sprite->bullet->x;
            //game_data->exp_y=game_data->sprite->bullet->y;
            game_data->score+=100;
            return 1;
        }
        else return 0;
    }
    else {
        game_data->score-=5;
        return 1;
    }
}

//0 if no collision, 1 otherwise
int CheckBulletCollision(struct game_data* game_data, Sprite *bullet, struct sllist *enemies) {
    struct lnode *node=enemies->head;
    Sprite *current = NULL;
    for(int i=0; i<enemies->size; i++) {
        current = (Sprite*)(node->data);
        if( (bullet->x >= current->x+10 && bullet->x <= current->x+70)&&(bullet->y >=current->y && bullet->y <= current->y+60) ) {
            if(i==0) {
                erase_drawn_sprite(current);
                sllist_pop_front(enemies);
            }
            if(i==enemies->size-1) {
                erase_drawn_sprite(current);
                sllist_pop_back(enemies);
            }
            else {
                erase_drawn_sprite(current);
                sllist_extract_after(enemies,i-1);
            }
            game_data->exp_x=current->x;
            game_data->exp_y=current->y;
            return 1;
        }
    node=node->next;
    }
    return 0;
}












