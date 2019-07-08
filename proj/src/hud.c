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
#include "sprite.h"

void draw_player_lives(struct game_data* game_data) {
    int x=974, y=722;
    if(game_data->state==GAME || game_data->state==RESPAWN) {
        for(int i=0; i<game_data->player_lives; i++) {
            draw_sprite(game_data->sprite->hearts[i],x,y);
            x-=60;
        }
    }
    return;
}

void print_endgame_screen(struct game_data* game_data) {
    switch(game_data->state) {
        case GAMELOST:
            draw_sprite(game_data->sprite->gamelost,306,80);
            break;
        case GAMEWON:
            draw_sprite(game_data->sprite->gamewon,311,80);
            break;
    }
    return;
}

void erase_drawn_score() {
    Sprite *dummy = create_sprite(score_xpm,5+149+1,722,0,0);
    erase_drawn_sprite(dummy);
    destroy_sprite(dummy);
    return;
}

void erase_drawn_date(){
    Sprite* dummy = create_sprite(score_xpm, 5, 0, 0, 0);
    erase_drawn_sprite(dummy);
    dummy->y +=5;
    erase_drawn_sprite(dummy);
    dummy->y -=5;
    dummy->x += 130;
    erase_drawn_sprite(dummy);
    dummy->y +=5;
    erase_drawn_sprite(dummy);
    dummy->y -=5;
    dummy->x+=130;
    erase_drawn_sprite(dummy);
    dummy->y +=5;
    erase_drawn_sprite(dummy);
    destroy_sprite(dummy);
    return;
}

void display_number(struct game_data* game_data, int n, int *x, int y) {
    char* n_string = itoa(n);
    for(int i=0; i<(int)strlen(n_string); i++) {
        switch(n_string[i]) {
            case '0':
                draw_sprite(game_data->sprite->number[0],*x,y);
                *x+=31+1;
                break;
            case '1':
                draw_sprite(game_data->sprite->number[1],*x,y);
                *x+=14+1;
                break;
            case '2':
                draw_sprite(game_data->sprite->number[2],*x,y);
                *x+=31+1;
                break;
            case '3':
                draw_sprite(game_data->sprite->number[3],*x,y);
                *x+=31+1;
                break;
            case '4':
                draw_sprite(game_data->sprite->number[4],*x,y);
                *x+=31+1;
                break;
            case '5':
                draw_sprite(game_data->sprite->number[5],*x,y);
                *x+=31+1;
                break;
            case '6':
                draw_sprite(game_data->sprite->number[6],*x,y);
                *x+=31+1;
                break;
            case '7':
                draw_sprite(game_data->sprite->number[7],*x,y);
                *x+=31+1;
                break;
            case '8':
                draw_sprite(game_data->sprite->number[8],*x,y);
                *x+=31+1;
                break;
            case '9':
                draw_sprite(game_data->sprite->number[9],*x,y);
                *x+=31+1;
                break;
            }    
    }
}

void print_score(struct game_data *game_data) {
    int x=5+149+3;
    draw_sprite(game_data->sprite->score,5,722);
    display_number(game_data, game_data->score,&x,722);
    return;
}

void display_date(struct game_data* game_data) {
    int x=5;
    int day=current_day();
    int month=current_month();
    int year=current_year();
    int hours=current_hours();
    int minutes=current_minutes();
    int seconds=current_seconds();

    for(int i=0; i<12; i++) {
        switch(i) {
            case 1: 
                display_number(game_data,day,&x,5);
                break;
            case 2: 
                draw_sprite(game_data->sprite->slash,x,5);
                x+=28;
                break;
            case 3: 
                display_number(game_data,month,&x,5); 
                break;
            case 4: 
                draw_sprite(game_data->sprite->slash,x,5);
                x+=28;
                break;
            case 5: 
                display_number(game_data,year,&x,5);
                break;
            case 6: 
                x+=31;
                break;
            case 7: 
                display_number(game_data,hours,&x,5);
                break;
            case 8: 
                draw_sprite(game_data->sprite->colon,x+5,15);
                x+=22;
                break;
            case 9: 
                display_number(game_data,minutes,&x,5);
                break;  
            case 10:
                draw_sprite(game_data->sprite->colon,x+5,15);
                x+=22;
                break;
            case 11:
                display_number(game_data,seconds,&x,5);
                 break;
        }
    }
    return;
}





