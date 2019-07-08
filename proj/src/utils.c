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

void make_negative(int* x) {
    int n=*x;
    if(*x > 0) n=0-n;
    *x=n;
    return;
}
void make_positive(int* x) {
    int n=*x;
    if(*x < 0) n=0-n;
    *x=n;
    return;
}

char* int_to_array(int number) {
    int n = log10(number) + 1;
    char *array = calloc(n,sizeof(char));
    for(int i=0; i<n; i++,number/=10) {
        array[i] = number % 10;
    }
    return array;
}

//returns 1 if out of bounds and 0 if within bounds
int CheckMoveXBoundaries(Sprite *sp) {
    int xf = sp->x+sp->xspeed;
    if( xf > 1024 || xf < 0 || xf + sp->width > 1024) {
        if(sp->xspeed > 0) return OFF_BOUNDS_R;
        if(sp->xspeed < 0) return OFF_BOUNDS_L;
    }
    return IN_BOUNDS;
}


//returns 1 if out of bounds and 0 if within bounds
int CheckMoveYBoundaries(Sprite* sp) {
    int yf = sp->y+sp->yspeed;
    if( yf > 768 || yf < 0 || yf + sp->height > 768) return 1;
    else return 0;
}

void fill_exp_array_metadata(struct game_data* game_data, int x, int y) {
    for(int i=0; i<9; i++) {
        game_data->sprite->explosion[i]->x=x;
        game_data->sprite->explosion[i]->y=y;
    }
    return;
}
