#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

//Additional functions/variables (code at the bottom):
int get_timer_adress(int timer_n);
static unsigned long handler_counter=0;
static int hook_id=0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  //declaring variables which will be used later
  uint8_t st, LSB, MSB, control_word=0;
  uint16_t load_value;
  //checking if freq is invalid, that is, less than 19. This value was calculated by dividing TIMER_FREQ
  //by the biggest 16 bit number possible. Since 1193182/65535 is around 18.2, rounding up the number
  //we get the smallest frequency possible which is 19Hz
  if(freq < 19) {
    printf("Invalid frequency input. Mustn't be less than 19\n");
    return -1;
  }
  //checking if timer index input is valid
  if(timer!=0 && timer!=1 && timer!=2) {
    printf("Timer does not exist. Input valid timer index\n");
    return -1;
  }
  //getting config from timer using previously implemented function
  if(timer_get_conf(timer,&st)==1) {
    printf("Error getting config from timer\n");
    return -1;
  }
  //setting right bits in control word depending on timer index
  switch(timer){
    case 0:
      control_word=TIMER_SEL0 | TIMER_LSB_MSB;
      break;
    case 1:
      control_word=TIMER_SEL1 | TIMER_LSB_MSB;
      break;
    case 2:
      control_word=TIMER_SEL2 | TIMER_LSB_MSB;
      break;
  }
  //preserving operating and counting mode and setting respective bits in control words
  control_word|=((BIT(3) | BIT(2) | BIT(1) | BIT(0)) & st );
  //writing control word to control register
  if(sys_outb(TIMER_CTRL, control_word)!=OK) {
    printf("Error writing control word to control register\n");
    return -1;
  }
  //getting value we have to load into timer to get desired frequency
  load_value = TIMER_FREQ / freq;
  //splitting load_value into respective LSB and MSB to be able to load it to timer
  if(util_get_LSB(load_value,&LSB)!=0) {
    printf("Error getting LSB to load into timer\n");
    return -1;
  }
  if(util_get_MSB(load_value,&MSB)!=0) {
    printf("Error getting MSB to load into timer\n");
    return -1;
  }
  //loading previously calculated bytes into timer
  if(sys_outb(get_timer_adress(timer),LSB)!=OK) {
    printf("Error loading LSB into timer\n");
    return -1;
  }
  if(sys_outb(get_timer_adress(timer),MSB)!=OK) {
    printf("Error loading MSB into timer\n");
    return -1;
  }
  //if everything went as expected, we get here and return 0
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  //passing hook_id by reference to variable in function parameters
  *bit_no=BIT(hook_id);
  //subscribing interrupt notification and enabling interrupts
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)!=OK) {
		printf("sys_irqsetpolicy failed\n");
		return -1;
	}
  //if everything went as expected, we get here and return 0
  return 0;
}

int (timer_unsubscribe_int)() {
  //unsubscribing interrupt notification and disabling interrupts
  if(sys_irqrmpolicy(&hook_id)!=OK) {
    printf("sys_irqrmpolicy failed\n");
    return -1;
  }
  //if everything went as expected, we get here and return 0
  return 0;
}

void (timer_int_handler)() {
  //incrementing counter used in timer_test_int in order to be able to successfully
  //count elapsed time
  handler_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //checking if timer index input is valid
  if(timer!=0 && timer!=1 && timer!=2) {
    printf("Timer does not exist. Input valid timer index.\n");
    return -1;
  }
  //writing read-back command control word to control register
  if(sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer))!=OK) {
    printf("Error writing to the Control register.\n");
    return -1;
  }
  //Reading output from timer with config info
  uint32_t aux;
  if(sys_inb(get_timer_adress(timer), &aux)!=OK) {
    printf("Error getting output with config information from timer register.\n");
    return -1;
  }
  //typecast of auxiliar variable and return intended timer info by reference
  *st=(uint8_t)aux;
  //if everything went as expected, we get here and return 0
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  //declaring union variable used later in print function
  union timer_status_field_val status;
  //loading (only) asked timer config info into union variable
  switch(field) {
    //if asked for all info, set union variable to whole status byte
    case all:
      status.byte=st;
      break;
    //if asked for initialization mode only, set union variably to in.mode and load
    //respective value (group added macros in i8254.h)
    case initial:
      if (STBT_INIT_LSB_MSB) status.in_mode=MSB_after_LSB;
      else if(STBT_INIT_LSB) status.in_mode=LSB_only;
      else if(STBT_INIT_MSB) status.in_mode=MSB_only;
      else/***************/ {status.in_mode=INVAL_val; printf("Invalid values in Status byte\n");}
      break;
    //if asked for counting mode only, set union variable to cound_mode and load
    //respective value (group added macros in i8254.h)
    case mode:
      if/***/(STBT_MODE==0) status.count_mode=0;
      else if(STBT_MODE==1) status.count_mode=1;
      else if(STBT_MODE==2) status.count_mode=2;
      else if(STBT_MODE==3) status.count_mode=3;
      else if(STBT_MODE==4) status.count_mode=4;
      else if(STBT_MODE==5) status.count_mode=5;
      else {printf("Invalid Status byte value\n");return -1;}
      break;
    //again, if asked for counting base only, set union variable to bcd and load
    //respective value (group added macros in i8254.h)
    case base:
      if(STBT_BASE_BIN) status.bcd=false;
      else/***********/ status.bcd=true;
      break;
  }
  //using already implemented print function to display results
  timer_print_config(timer,field,status);
  //if everything went as expected, we get here and return 0
  return 0;
}

//Additional functions:
int get_timer_adress (int timer_n) {
  if(timer_n==0)/***/ return TIMER_0;
  else if(timer_n==1) return TIMER_1;
  else if(timer_n==2) return TIMER_2;
  else/*************/ return 0;
}
