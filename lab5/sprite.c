// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "sprite.h"
#include "video_card.h"

static int nframes = 0;
int nframes_required;

Sprite * create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed) {
    //allocate space for the "object"
    Sprite*sp = (Sprite*) malloc ( sizeof(Sprite));
    if( sp == NULL ) return NULL;

    //read the sprite mixmap
    sp->map = read_xpm(pic,&(sp->width),&(sp->height));
    if(sp->map==NULL) { //if pixmap is invalid, we free previously allocated space
        free(sp);
        return NULL;
    }

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
            if(draw_pixel( (sp->x)+i, (sp->y)+j, 0)!=0) {
                printf("erase_drawn sprite :: draw_pixel error\n");
                return;
            }
        }
    }
    //printf("\n x: %d; y: %d \n",sp->x,sp->y);
    return;
}

int move_sprite(Sprite *sp, int xf, int yf) {
    nframes++;
    if(nframes < nframes_required){
        return -1;
    }
    nframes = 0;
    erase_drawn_sprite(sp);

    sp->x += sp->xspeed;
    sp->y += sp->yspeed;
    if(sp->xspeed > 0) {
        if (sp->x >= xf) {

         sp->x = xf;
         sp->y = yf;
         //Drawing the sprite one last time before exiting the drawing loop (returning 0)
         draw_sprite(sp, sp->x, sp->y);
         return 0;
        }
    } else if(sp->yspeed > 0){
        if (sp->y >= yf) {

         sp->x = xf;
         sp->y = yf;
         //Drawing the sprite one last time before exiting the drawing loop (returning 0)
         draw_sprite(sp, sp->x, sp->y);

        return 0;
        }
    } else if (sp->xspeed < 0) {
        if (sp->x <= xf) {
         sp->x = xf;
         sp->y = yf;
         //Drawing the sprite one last time before exiting the drawing loop (returning 0)
         draw_sprite(sp, sp->x, sp->y);

         return 0;
        }
     } else if(sp->yspeed < 0) {
         if (sp->y <= yf) {
         sp->x = xf;
         sp->y = yf;
         //Drawing the sprite one last time before exiting the drawing loop (returning 0)
         draw_sprite(sp, sp->x, sp->y);

         return 0;
     }
    }

    draw_sprite(sp, sp->x, sp->y);
    return -1;
}






