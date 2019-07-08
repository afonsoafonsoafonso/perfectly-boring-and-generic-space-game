// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "video_card.h"
#include "keyboard.h"
#include "sprite.h"
extern uint8_t scancode;
extern int nframes_required;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) {
  if(vg_init(mode)==NULL) {
    printf("video_test_init :: vg_init failed\n");
    return -1;
  }

  sleep(delay);

  vg_exit();

  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  int irq_bitmask =  keyboard_subscribe();
  int r;
  int ipcStatus;
  message msg; 
  int esc = 0;
  //subscribing keyboard
  if (irq_bitmask==-1) {
    printf("video_test_rectangle :: Error subscribing keyboard interrutps\n");
    return -1;
  }
  //setting specified mode
  if(vg_init(mode)==NULL) {
    printf("video_test_rectangle :: Error vg_init\n");
    return -1;
  }
  vg_draw_rectangle(x,y,width,height,color);

  while(esc==0) {
   if((r=driver_receive(ANY,&msg,&ipcStatus))!=0) {
     printf("driver_receive failed with: %d", r);
     continue;
   }
   if(is_ipc_notify(ipcStatus)) {
     switch(_ENDPOINT_P(msg.m_source)) {
       case HARDWARE:
         if(msg.m_notify.interrupts & irq_bitmask) {
           (kbc_ih)();
           if(scancode == ESC_BREAK){
             esc = 1;
           }
         }
         break;
      default:
          break;
     }
   }
  }
  if(keyboard_unsubscribe() != 0){
    return -1;
  }

  vg_exit();
  return 0;
}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  int irq_bitmask =  keyboard_subscribe();
  int r;
  int ipcStatus;
  message msg; 
  int esc = 0;
  //subscribing keyboard
  if (irq_bitmask==-1) {
    printf("video_test_rectangle :: Error subscribing keyboard interrutps\n");
    return -1;
  }
  //setting specified mode
  if(vg_init(mode)==NULL) {
    printf("video_test_rectangle :: Error vg_init\n");
    return -1;
  }

  //vg_draw_rectangle(x,y,width,height,color);
  vbe_mode_info_t vbe_mode_info;
  if(get_mode_info(mode,&vbe_mode_info)!=0) {
       printf("video_test_pattern :: error in get_mode_info\n");
       return -1;
  }
  uint16_t width=0, height=0, bits_per_pixel; uint32_t color;
  width= vbe_mode_info.XResolution / no_rectangles;
  height= vbe_mode_info.YResolution / no_rectangles;
  bits_per_pixel= vbe_mode_info.BitsPerPixel;

  if(bits_per_pixel==8) {
    for(int row=0; row<no_rectangles; row++) {
      for(int col=0; col<no_rectangles; col++ ) {
        color=(first + (row * no_rectangles + col) * step) % (1 << vbe_mode_info.BitsPerPixel);
        vg_draw_rectangle(col*width,row*height,width,height,color);
      }
    }
  }
  else {
    uint32_t R, G, B;
    uint32_t R_first=first >> vbe_mode_info.RedFieldPosition % BIT(vbe_mode_info.RedMaskSize);
    uint32_t G_first=first >> vbe_mode_info.GreenFieldPosition % BIT(vbe_mode_info.GreenMaskSize);
    uint32_t B_first=first >> vbe_mode_info.BlueFieldPosition % BIT(vbe_mode_info.BlueMaskSize);
    printf("RF: %x ; GF: %x ; BF: %x\n",R_first, G_first, B_first);
    printf("first: %x\n", first);

    for(int row=0; row<no_rectangles; row++) {
      for(int col=0; col<no_rectangles; col++) {
        R=(R_first + col*step) % (1 << vbe_mode_info.RedMaskSize);
        G=(G_first + row*step) % (1 << vbe_mode_info.GreenMaskSize);
        B=(B_first + (col+row)*step) % (1 << vbe_mode_info.BlueMaskSize);
        printf("%x ; %x ; %x\n",R,G,B);
        vg_draw_rectangle(col*width,row*height,width,height, (R << vbe_mode_info.RedFieldPosition) +
                                            (G << vbe_mode_info.GreenFieldPosition)+
                                            (B << vbe_mode_info.BlueFieldPosition));
      }
    }
  }

  while(esc==0) {
   if((r=driver_receive(ANY,&msg,&ipcStatus))!=0) {
     printf("driver_receive failed with: %d", r);
     continue;
   }
   if(is_ipc_notify(ipcStatus)) {
     switch(_ENDPOINT_P(msg.m_source)) {
       case HARDWARE:
         if(msg.m_notify.interrupts & irq_bitmask) {
           (kbc_ih)();
           if(scancode == ESC_BREAK){
             esc = 1;
           }
         }
         break;
       default:
          break;
     }
   }
  }
  if(keyboard_unsubscribe() != 0){
    return -1;
  }
  vg_exit();
  return 0;
}

int (video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y) {
  int r;
  int ipcStatus;
  message msg; 
  int esc = 0;
  //subscribing keyboard
  int irq_bitmask =  keyboard_subscribe();
  if (irq_bitmask==-1) {
    printf("video_test_rectangle :: Error subscribing keyboard interrutps\n");
    vg_exit();
    return -1;
  }
  //setting 0x105 mode
  if(vg_init(0x105)==NULL) {
    printf("video_test_rectangle :: Error vg_init\n");
    vg_exit();
    return -1;
  }

  if(vg_draw_xpm(xpm,x,y)!=0) {
    printf("video_test_xpm :: vg_draw_xpm error\n");
    vg_exit();
    return -1;
  }

  while(esc==0) {
   if((r=driver_receive(ANY,&msg,&ipcStatus))!=0) {
     printf("driver_receive failed with: %d", r);
     continue;
   }
   if(is_ipc_notify(ipcStatus)) {
     switch(_ENDPOINT_P(msg.m_source)) {
       case HARDWARE:
         if(msg.m_notify.interrupts & irq_bitmask) {
           (kbc_ih)();
           if(scancode == ESC_BREAK){
             esc = 1;
           }
         }
         break;
       default:
          break;
     }
   }
  }
  if(keyboard_unsubscribe() != 0){
    vg_exit();
    return -1;
  }
  vg_exit();
  return 0;
}

int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  uint8_t timer_irq=0;
  timer_subscribe_int(&timer_irq);
  int r;
  int ipcStatus;
  message msg; 
  int esc = 0;
  int axis_speed;
  //testing if frame rate and speed are valid
  if(fr_rate > 60) {
    printf("Invalid frame rate. Maximum refresh rate is 60\n");
    return -1;
  }
  if(speed==0) {
    printf("Invalid speed. \n");
    return -1;
  }
  //subscribing keyboard
  int irq_bitmask =  keyboard_subscribe();
  if (irq_bitmask==-1) {
    printf("video_test_rectangle :: Error subscribing keyboard interrutps\n");
    vg_exit();
    return -1;
  }
  //setting 0x105 mode
  if(vg_init(0x105)==NULL) {
    printf("video_test_rectangle :: Error vg_init\n");
    vg_exit();
    return -1;
  }
  //creating sprite and speed calculatings
  Sprite *sp;
  if(yi==yf) {
    if(speed > 0) {
      axis_speed=abs(speed)*(abs(xf-xi)/(xf-xi));
      sp=create_sprite(xpm,xi,yi,axis_speed,0);
    }
    else {
      axis_speed=abs(xf-xi)/(xf-xi);
      sp=create_sprite(xpm,xi,yi,axis_speed,0);
      nframes_required = abs(speed);
    }
  }
  else {
    if(speed > 0) {
      axis_speed=abs(speed)*abs(yf-yi)/(yf-yi);
      sp=create_sprite(xpm,xi,yi,0,axis_speed);
    }
    else {
      axis_speed=abs(speed)*abs(yf-yi)/(yf-yi);
      sp=create_sprite(xpm,xi,yi,0,axis_speed);
      nframes_required = abs(speed);
    }
  }

  draw_sprite(sp, xi, yi);
  //calculating how many interrupts a frame will take
  int ints_per_frame=sys_hz()/fr_rate;
  int int_counter=0;
  
  while(esc==0) {
   if((r=driver_receive(ANY,&msg,&ipcStatus))!=0) {
     printf("driver_receive failed with: %d", r);
     continue;
   }
   if(is_ipc_notify(ipcStatus)) {
     switch(_ENDPOINT_P(msg.m_source)) {
       case HARDWARE:
         if(msg.m_notify.interrupts & irq_bitmask) {
           (kbc_ih)();
           if(scancode == ESC_BREAK){
             esc = 1;
           }
         }
         if(msg.m_notify.interrupts & timer_irq) {
           int_counter++;
            if(int_counter % ints_per_frame == 0){
              if((sp->x != xf && sp->y == yf) || (sp->x == xf && sp->y != yf)){
                if(speed<0) {
                  move_sprite(sp,xf, yf);
                }
                if(speed>0) {
                  move_sprite(sp,xf,yf);
                }
              }
           }
         }
         break;
       default:
          break;
     }
   }
  }

  keyboard_unsubscribe();
  timer_unsubscribe_int();
  destroy_sprite(sp);
  vg_exit();

  return 0;
}

int (video_test_controller)(){
  if(vbe_get_control_info() != 0){
    return -1;
  }
  return 0;
}


