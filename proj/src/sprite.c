// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "sprite.h"
#include "video_card.h"
#include "../res/xpm.h"

Sprite * create_sprite(xpm_string_t* pic, int x, int y, int xspeed, int yspeed) {
    xpm_image_t test;
    //allocate space for the "object"
    Sprite*sp = (Sprite*) malloc ( sizeof(Sprite));
    if( sp == NULL ) return NULL;

    //read the sprite mixmap
    sp->map = (uint16_t*)xpm_load(pic,XPM_5_6_5,&test);
    if(sp->map==NULL) { //if pixmap is invalid, we free previously allocated space
        free(sp);
        return NULL;
    }

    sp->height=test.height;
    sp->width=test.width;
    sp->x=x;
    sp->y=y;
    sp->xspeed=xspeed;
    sp->yspeed=yspeed;

    return sp;
}

void draw_sprite(Sprite *sp, int x, int y) {
    for(int i=0; i<sp->width; i++) {
        for(int j=0; j<sp->height; j++) {
            if(draw_pixel(x+i,y+j,sp->map[j * sp->width + i])!=0) {
                printf("vg_draw_xpm :: draw_pixel error\n");
                return;
            }
        }
    }
    return;
}

void destroy_sprite(Sprite *sp) {
    if(sp==NULL) return;
    if(sp->map) free(sp->map);
    free(sp);
    return;
}

void erase_drawn_sprite(Sprite *sp) {
    //clearing sprite 
    for(int i=0; i < /*sp->x +*/sp->width; i++) {
        for(int j=0; j< /*sp->y +*/ sp->height; j++) {
            if(draw_pixel( (sp->x)+i, (sp->y)+j,background->map[((j+ sp->y) * background->width)+ (sp->x) + i])!=0) {
                //printf("erase_drawn sprite :: draw_pixel error\n");
                return;
            }
        }
    }
    return;
}


