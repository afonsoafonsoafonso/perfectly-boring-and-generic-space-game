// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "i8042.h"

extern uint8_t scancode;
extern int sys_cnt;
extern unsigned long handler_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (kbd_test_scan)(bool assembly) {
  int irq_set = keyboard_subscribe(), ipc_status, r;
  message msg;
  //testing if keyboard_subcribe was successful
  if(irq_set == -1) {
    printf("Error subscribing to interrupt notifications\n");
    return -1;
  }
  //while ESC key isnt pressed, we continue
  while(scancode!=ESC_BREAK) {
    if((r=driver_receive(ANY,&msg,&ipc_status))!=0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if(msg.m_notify.interrupts & irq_set) {
            if( assembly == false){
              (kbc_ih)();
            }
            else{
              (kbc_asm_ih)();
            }
            print_scancode(&scancode);
          }
      }
    }
  }
  if(keyboard_unsubscribe()!=0) {
    printf("Error unsubscribing\n");
  }
  if(assembly == false){
    kbd_print_no_sysinb(sys_cnt);
  }
  return 0;
}

int (kbd_test_poll)() {
  uint8_t cmd_mask=0, scancode_pol=0;
  int read_out_buf_error;
  //loop to do while esc hasnt been pressed and released yet
  while(scancode_pol!=ESC_BREAK) {
    //reading output buffer and saving it in scancode variable
    read_out_buf_error=kbc_read_out_buf(&scancode_pol);
    //analyzing kbc_read_out_buf return
    switch(read_out_buf_error) {
      case 1:
        //printf("No data was available to be read. Retry\n");
        continue;
      case -1:
        printf("Error reading status byte\n");
        break;
      case -2:
        printf("Error reading output buffer\n");
        break;
      case -3:
        printf("Parity, timeout or aux error\n");
        break;
    }
  print_scancode(&scancode_pol);
  }
  //saving command byte so we can enable interrupts
  if(kbc_write_cmd(KBC_CMD_REG,KBC_READ_COMBYTE)!=0) {
    printf("Error writing command to command register asking to read command byte\n");
    return -1;
  }
  //reading output buffer with (hopefully) the current command byte
  if(kbc_read_out_buf(&cmd_mask)!=0) {
    printf("Error reading output buffer. \n");
    return -1;
  }
  //creating new mask
  cmd_mask|=KBC_CB_INT;
  //writing to the command register asking to write command byte
  if(kbc_write_cmd(KBC_CMD_REG,KBC_WRITE_COMBYTE)!=0) {
    printf("Error writing command to command register asking to write command byte\n");
    return -1;
  }
  //finally, writing new command byte and enabling interrupts
  if(kbc_write_cmd(KBC_CMD_ARGS,cmd_mask)!=0) {
    printf("Error writing command byte to the register\n");
    return -1;
  }
  //printing number of sys kernel calls
  kbd_print_no_sysinb(sys_cnt);
  return 0;
}

int (kbd_test_timed_scan)(uint8_t n) {
  int keyboard_irq_set=keyboard_subscribe();
  uint8_t timer_irq_set;
  int driver_receive_error=0;
  int ipc_status;
  message msg;
  timer_subscribe_int(&timer_irq_set);
  //while n seconds havent passed without input or escape hasnt been pressed
  while(handler_counter < n*sys_hz() && scancode!=ESC_BREAK) {
    if((driver_receive_error=driver_receive(ANY,&msg,&ipc_status))!=0) {
      printf("driver_receive failed with %d", driver_receive_error);
      continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        //if it was an hardware notification
        case HARDWARE:
          //if it was a timer notification, we increase the counter
          if(msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
          }
          //if it was a keyboard notification, we reset the timer counter
          //and print the scancode that was received
          if(msg.m_notify.interrupts & keyboard_irq_set) {
            kbc_ih();
            print_scancode(&scancode);
            handler_counter=0;
          }
          break;
          //we dont expect any other type of notification so we do nothing
        default:
          break;
      }
    }
    else {}
  }
  //unsubscribing timer notifications and disabling irq line
  if(timer_unsubscribe_int()!=0) {
    printf("Error unsubscribing timer\n");
    return -1;
  }
  //unsubscribing keyboard notifications and disabling irq line
  if(keyboard_unsubscribe()!=0) {
    printf("Error unsubscribing keyboard\n");
    return -1;
  }
  //checking and printing the reason the while above as breaked
  if(scancode==ESC_BREAK) {
    printf("Escape pressed\n");
  }
  else {
    printf("keyboard input timedout. %d seconds passed without input\n", n);
  }
  return 0;
}
