#include <lcom/lcf.h>

#include <lcom/lab2.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

extern unsigned long handler_counter;
extern int hook_id;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t st=0;
  if(timer_get_conf(timer,&st)!=0) return -1;
  if(timer_display_conf(timer,st,field)!=0) return -1;
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if(timer_set_frequency(timer,freq)!=0) return -1;
  return 0;
}

int(timer_test_int)(uint8_t time) {
  //declaring variables used later
  int ipc_status;
  int driver_receive_error;
  uint8_t irq_set=0;
  message msg;
  //checking if timer input is valid
  if(time<=0) {
    printf("Invalid time input. Must be greater than zero\n");
    return -1;
  }
  //subscribing timer
  if(timer_subscribe_int(&irq_set)!=0) {
    printf("Error subscribing timer\n");
    return -1;
  }
  //loop to be repeated while total time hasnt passed
  while(handler_counter < time*sys_hz()) {
  //following code explained in slide 19 of I/O Interrupts .ppt
    if((driver_receive_error=driver_receive(ANY,&msg,&ipc_status))!=0) {
      printf("driver_receive failed with: %d", driver_receive_error);
      continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {

        case HARDWARE:
          if(msg.m_notify.interrupts & irq_set) {
            timer_int_handler();
            if(handler_counter % 60 == 0) {
              timer_print_elapsed_time();
            }
          }
          break;

        default:
           break;
      }
    }
    else {}
  }
  //unsubscribing timer
  if(timer_unsubscribe_int()!=0) {
    printf("Error unsubscribing timer\n");
    return -1;
  }
  //if everything went as expected, we get here and return 0
  return 0;
}
//function to extract least significant byte from 2byte value
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb=0;
  *lsb=(uint8_t)val;
  return 0;
}
//function to extract most significant byte from 2byte value
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb=0;
  *msb=(val >> 8);
  return 0;
}
