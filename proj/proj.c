// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "src/int_handlers.h"
#include "src/sllist.h"
#include "src/keyboard.h"
#include "src/sprite.h"
#include "src/player.h"
#include "res/xpm.h"
#include "src/mouse.h"
#include "src/i8042.h"
#include "src/i8254.h"
#include "src/game_setup.h"
#include "src/game_states.h"
#include "src/game_structs.h"
#include "src/video_card.h"
#include "src/rtc.h"

extern uint8_t scancode;
extern Sprite* background;
extern uint32_t mouse_data;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  int ipc_status, r;
  message msg;
  struct game_data *game_data=(struct game_data *)malloc(sizeof(struct game_data));
  game_data->sprite=(struct sprites *)malloc(sizeof(struct sprites));
  struct devices_data *io_data=(struct devices_data *)malloc(sizeof(struct devices_data));

  ints_init(io_data);
  vg_init(0x117);
  game_init(&background,game_data, io_data);
  
  //while ESC key isnt pressed, we continue
  while(game_data->state!=EXIT) {
    if( (r=driver_receive(ANY,&msg,&ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if(msg.m_notify.interrupts & io_data->keyboard_bitmask) {
            keyboard_handler(&scancode,game_data);
          }
          if (msg.m_notify.interrupts & io_data->mouse_bitmask) {
            mouse_handler(&io_data->byte_counter,mouse_data,&io_data->mouse_pp,game_data->sprite->cursor,game_data->sprite->player,game_data);
          }
          if(msg.m_notify.interrupts & io_data->timer_bitmask){
            timer_handler(&game_data->state,&scancode,io_data,game_data);
          }
           break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  game_exit(&background, game_data->sprite);

  return 1;
}
