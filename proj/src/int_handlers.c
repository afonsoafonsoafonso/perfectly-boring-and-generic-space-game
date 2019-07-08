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

//extern uint8_t scancode;
extern Sprite* background;
extern unsigned long handler_counter;
static int respawn_cnt=0;
//extern uint32_t mouse_data;

void keyboard_handler(uint8_t* scancode, struct game_data *game_data) {
    switch(game_data->state) {
        case MENU:
            kbc_ih();
            break;  

        case GAME:
            kbc_ih();
            player_move_kbc(game_data->sprite->player,scancode,game_data,KBC_INT);
            if(*scancode == SPACE_MAKE && game_data->bullet_flag==0) {
                game_data->bullet_flag=1;
                game_data->sprite->bullet->x=game_data->sprite->player->x+31;
                game_data->sprite->bullet->y=game_data->sprite->player->y-13;
                draw_sprite(game_data->sprite->bullet,game_data->sprite->bullet->x,game_data->sprite->bullet->y);
                draw_sprite(game_data->sprite->player, game_data->sprite->player->x, game_data->sprite->player->y);
            }
            else if(*scancode == ESC_BREAK) {
                game_data->state=PAUSE;
            }
            break;

        case RESPAWN:
            kbc_ih();
            if(*scancode == 0x48 || *scancode == 0x50 || *scancode== 0x4d || *scancode== 0x4b || *scancode == SPACE_MAKE) {
                game_data->state=GAME;
            }
            break;

        case GAMELOST:
            kbc_ih();
            break;

        case GAMEWON:
            kbc_ih();
            player_move_kbc(game_data->sprite->player,scancode,game_data,KBC_INT);
            if(*scancode == SPACE_MAKE && game_data->bullet_flag==0) {
                game_data->bullet_flag=1;
                game_data->sprite->bullet->x=game_data->sprite->player->x+31;
                game_data->sprite->bullet->y=game_data->sprite->player->y-13;
                draw_sprite(game_data->sprite->bullet,game_data->sprite->bullet->x,game_data->sprite->bullet->y);
            }
            break;
        
        case PAUSE:
            kbc_ih();
            if(*scancode == ESC_BREAK) {
                erase_drawn_date();
                game_data->state=GAME;
            }
            break;
    }
    return;
}

void mouse_handler(int* bytecounter, uint32_t mouse_data, struct packet* pp, Sprite* cursor, Sprite* player, struct game_data* game_data) {
    switch(game_data->state) {
        case MENU:
            mouse_ih();
            if(*bytecounter==0 && (mouse_data & BIT(3)) ==0) {
                *bytecounter=0;
            }
            else {
                pp->bytes[*bytecounter] = mouse_data;
                (*bytecounter)++;
            }
            if(*bytecounter==3) {
                struct_packet_filler(pp);
                cursor_move(cursor,pp);
                check_button_click(pp, cursor, &game_data->state);
                *bytecounter=0;
            }
            break;
        case GAME:
            mouse_ih();
            if(*bytecounter==0 && (mouse_data & BIT(3)) ==0) {
                *bytecounter=0;
            }
            else {
                pp->bytes[*bytecounter] = mouse_data;
                (*bytecounter)++;
            }
            if(*bytecounter==3) {
                struct_packet_filler(pp);
                player_move_mouse(player,pp);
                *bytecounter=0;
            }
            break;
        case RESPAWN:
            mouse_ih();
            if(*bytecounter==0 && (mouse_data & BIT(3)) ==0) {
                *bytecounter=0;
            }
            else {
                pp->bytes[*bytecounter] = mouse_data;
                (*bytecounter)++;
            }
            if(*bytecounter==3) {
                struct_packet_filler(pp);
                if(pp->delta_x!=0 || pp->delta_y!=0) {
                    game_data->state=GAME;
                }
                *bytecounter=0;
            }
            break;
        case GAMELOST:
            mouse_ih();
            break;
        case GAMEWON:
            mouse_ih();
            if(*bytecounter==0 && (mouse_data & BIT(3)) ==0) {
                *bytecounter=0;
            }
            else {
                pp->bytes[*bytecounter] = mouse_data;
                (*bytecounter)++;
            }
            if(*bytecounter==3) {
                struct_packet_filler(pp);
                player_move_mouse(player,pp);
                *bytecounter=0;
            }
            break;
        case PAUSE:
            mouse_ih();
            break;
    }
    return;
}

void timer_handler(int *state, uint8_t* scancode ,struct devices_data *io_data, struct game_data *game_data) {
    switch(*state) {
        case MENU:

            break;
            
        case GAME:
            erase_drawn_score(game_data);
            (io_data->timer_int_cnt)++;
            timer_int_handler();
            if(io_data->timer_int_cnt % (sys_hz()/60) == 0) {
                if(game_data->bullet_flag==1) {
                    if(player_bullet_move(game_data)==1) {
                        erase_drawn_sprite(game_data->sprite->bullet);
                        game_data->bullet_flag=0;
                    }
                }
                enemy_explosion(game_data);
                player_move_kbc(game_data->sprite->player,scancode,game_data,TIMER_INT);
                enemy_move(game_data->sprite->enemies);
                enemy_fire(game_data);
                if(enemy_bullet_movement(game_data)==1 && game_data->invulnerability_flag==0) {
                    game_data->player_exp_x=game_data->sprite->player->x;
                    game_data->player_exp_y=game_data->sprite->player->y;
                    erase_drawn_sprite(game_data->sprite->hearts[game_data->player_lives-1]);
                    (game_data->player_lives)--;
                    if (game_data->player_lives!=0) *state=RESPAWN;
                    if(game_data->player_lives==0) {
                        *state=GAMELOST;
                    }
                    game_data->player_exp_flag=1;
                }
                player_explosion(game_data);
                draw_player_lives(game_data);
                print_score(game_data);
            }
            if(game_data->sprite->enemies->size==0) *state = GAMEWON;
            break;

        case RESPAWN:
            (io_data->timer_int_cnt)++;
            timer_int_handler();
            game_data->sprite->player->x=512;
            game_data->sprite->player->y=600;
            if(io_data->timer_int_cnt % (sys_hz()/60) == 0) {
                player_explosion(game_data);
                if(respawn_cnt % 50 ==0) draw_sprite(game_data->sprite->player,512,600);
                else erase_drawn_sprite(game_data->sprite->player);
                respawn_cnt++;

                if(game_data->bullet_flag==1) {
                    if(player_bullet_move(game_data)==1) {
                        erase_drawn_sprite(game_data->sprite->bullet);
                            game_data->bullet_flag=0;
                    }
                }
                enemy_explosion(game_data);
                enemy_move(game_data->sprite->enemies);
                enemy_fire(game_data);
                enemy_bullet_movement(game_data);
                draw_player_lives(game_data);
                print_score(game_data);
            }
            break;

        case GAMELOST:
            (io_data->timer_int_cnt)++;
            timer_int_handler();
            if(io_data->timer_int_cnt % (sys_hz()/60)==0) {
                player_explosion(game_data);
                enemy_explosion(game_data);
                enemy_move(game_data->sprite->enemies);
                enemy_fire(game_data);
                enemy_bullet_movement(game_data);
                print_endgame_screen(game_data);
                print_score(game_data);
            }
            if(*scancode == ESC_BREAK) {
                erase_drawn_score();
                menu_init(background,game_data->sprite->cursor);
                game_init(&background,game_data, io_data);
                *state=MENU;
            }
            break;
        
        case GAMEWON:
            (io_data->timer_int_cnt)++;
            timer_int_handler();
            if(io_data->timer_int_cnt % (sys_hz()/60) == 0) {
                if(game_data->bullet_flag==1) {
                    if(player_bullet_move(game_data)==1) {
                        erase_drawn_sprite(game_data->sprite->bullet);
                        game_data->bullet_flag=0;
                    }
                }
                //enemy_bullet_movement(game_data);
                enemy_explosion(game_data);
                player_move_kbc(game_data->sprite->player,scancode,game_data,TIMER_INT);
                player_explosion(game_data);
                draw_player_lives(game_data);
                print_endgame_screen(game_data);
                draw_player_lives(game_data);
                print_score(game_data);
                if(*scancode == ESC_BREAK) {
                    erase_drawn_score();
                    menu_init(background,game_data->sprite->cursor);
                    game_init(&background,game_data, io_data);
                    *state=MENU;
                }
            }
            break;

        case PAUSE:
            erase_drawn_date();
            display_date(game_data);
            break;
    }
    
    WriteToMainBuffer();
    return;
}




