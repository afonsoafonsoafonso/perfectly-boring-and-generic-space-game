#include <lcom/lcf.h>

#include "i8042.h"
#include "i8254.h"

//global variables for print_scancode
bool make, WasLastByte_e0;
uint8_t bytes[2];
//hook_id given to keyboard
int kbd_hook_id = 1;
//global variable to store scancode
uint8_t scancode;
//sys kernel calls counter
int sys_cnt;

int sys_inb_cnt(port_t port, uint32_t *byte) {
  sys_cnt++;
  return sys_inb(port,byte);
}

int read_stat_reg(uint32_t *byte) {
  if (sys_inb_cnt(KBC_STATUS_REG, byte) != OK) {
    return -1;
  }
  else return 0;
}

int keyboard_subscribe() {
  int temp = kbd_hook_id;
  //subscribing to interrupt notifications and enabling irq line
  if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)!=OK) {
    return -1;
  }
  //if everything went as expected
  return BIT(temp);
}

int keyboard_unsubscribe() {
  //unsubscribing interrupt nofitications and disabling irq line
  if(sys_irqrmpolicy(&kbd_hook_id)!=OK) {
    return -1;
  }
  //if everything went as expected
  return 0;
}

int kbc_write_cmd(uint8_t reg, uint32_t cmd) {
  uint32_t status;

  for (int n_tries = 0; n_tries < MAX_TRIES; n_tries++) {
    if (read_stat_reg(&status) != 0) {
      //error reading status register. retry
      continue;
    }
    //cheking if input buffer is full
    if ((status & KBC_IBF) == 0) {
      //if input buffer isnt full, we write to the register
      if (sys_outb(reg, cmd) != OK) {
        //error writing to registry. retry
        continue;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  //if it got here it means it wasnt possible to write command to given register
  printf("kbc command writing failed after %d tries\n", MAX_TRIES);
  return -1;
}

int kbc_read_out_buf(uint32_t *output) {
  uint32_t status;
  *output=0;

  for(int n_tries = 0; n_tries < MAX_TRIES; n_tries++) {
    if (read_stat_reg(&status) != 0) {
      //error reading status byte. retry?
      return -1;
    }
    //if data is available to be read
    if (status & KBC_OBF) {
      //if parity error, timeout error if it's mouse data instead of keyboard
      if ((status & (KBC_PARITY | KBC_TIMEOUT | KBC_AUX)) == 0) {
        if (sys_inb_cnt(KBC_OUT_BUF, output)!=OK) {
          //error reading output buffer. retry?
          return -2;
        }
        else {
          return 0;
        }
      }
      else {
        //error in parity,timeout or aux. retry?
        return -3;
      }
    }
    //if it got here it means nothing was read. wait and try again
    tickdelay(micros_to_ticks(DELAY_US));
  }
  //if nothing was read during MAX_TRIES no of tries
  //commented printf below because it just spammed the terminal when there wasnt input
  //printf("couldnt read output buffer. gave up after %d tries\n", MAX_TRIES);
  return 1;
}

void (kbc_ih)() {
  uint32_t status, data;
  //reading status byte
  if(read_stat_reg(&status)!=0) {
    printf("Error reading status byte\n");
  }
  //checking if output buffer has data to be read
  if(status & KBC_OBF) {
    //reading output buffer data
    if(kbc_read_out_buf(&data)!=0) {
      printf("error reading outpuf buffer\n");
    }
    else {
      //if output buffer was read successfuly, save data
      //in global variable
      scancode=data;
    }
  }
}

void print_scancode(uint8_t *kbc_scancode) {
  if (*kbc_scancode == 0xe0) {
    WasLastByte_e0 = true;
    bytes[0] = 0xe0;
  }
  //if scancode is makecode
  else if ((*kbc_scancode & BIT(7)) == 0) {
    make = true;
    //if scancode is makecode & 2 bytes
    if (WasLastByte_e0 == true) {
      bytes[1] = *kbc_scancode;
      kbd_print_scancode(make, 2, bytes);
      WasLastByte_e0 = false;
      bytes[0] = 0;
      bytes[1] = 0;
    }
    //if its only 1 byte
    else {
      bytes[0] = *kbc_scancode;
      kbd_print_scancode(make, 1, bytes);
      bytes[0] = 0;
      bytes[1] = 0;
    }
  }
  //if scancode is breakcode
  else {
    make = false;
    //if its breakcode & 2 bytes
    if (WasLastByte_e0 == true) {
      bytes[1] = *kbc_scancode;
      kbd_print_scancode(make, 2, bytes);
      WasLastByte_e0 = false;
      bytes[0] = 0;
      bytes[1] = 0;
    }
    //if its only 1 byte
    else {
      bytes[0] = *kbc_scancode;
      kbd_print_scancode(make, 1, bytes);
      bytes[0] = 0;
      bytes[1] = 0;
    }
  }
}
