// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "i8042.h"
//added group variables
bool gesture_finish;
static int mouse_hook_id = 12;
int erro = 0;
uint32_t mouse_data;

enum state {
  init,
  draw1,
  lb_to_rb,
  draw2,
  accept,
};
       
int kbc_and_mouse_reset();

int subscribe_mouse(){

  int bitmask = BIT(mouse_hook_id);

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)!= OK) {
	  return -1;
  }

    return bitmask;
}

int mouse_disable_int(){
  if(sys_irqdisable(&mouse_hook_id) != OK){
    printf("error in disabling notifications\n");
    return -1;
  }
  return 0;
}

int mouse_enable_int(){
  if(sys_irqenable(&mouse_hook_id) != OK){
    printf("error in enabling notifications\n");
    return -1;
  }
  return 0;
}

int mouse_read_out_buf(uint32_t *output) {
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
      if ((status & (KBC_PARITY | KBC_TIMEOUT)) == 0) {
        if (sys_inb(KBC_OUT_BUF, output)!=OK) {
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

int mouse_write_byte(uint8_t cmd) {
  uint32_t local_output;
  while(1) {
    if(kbc_write_cmd(KBC_CMD_REG,MOUSE_WRITE_BYTE)!=0) {
      printf("Error asking register to write byte to mouse\n");
      return -1;
    }


    if(kbc_write_cmd(KBC_CMD_ARGS,cmd)!=0) {
      printf("Error writing byte to mouse\n");
      return -1;
    }
    mouse_read_out_buf(&local_output);
    if(local_output==MOUSE_ACK) break;
    else if(local_output==MOUSE_NACK) continue;
    else {
      printf("Error in acknowledge_byte\n");
      mouse_write_byte(cmd);
      return -1;
    }
  }
  return 0;
}

int kbc_and_mouse_reset() {
  //uint8_t local_output;
  mouse_write_byte(MOUSE_SET_STREAM_MODE);
  mouse_write_byte(MOUSE_DISABLE_DATA_REPORTING);
  /*while(1) {
    if(kbc_write_cmd(KBC_CMD_REG,MOUSE_WRITE_BYTE)!=0) {
      printf("Error asking register to write byte to mouse\n");
      return -1;
    }
    if(kbc_write_cmd(KBC_CMD_ARGS,MOUSE_SET_STREAM_MODE)!=0) {
      printf("Error writing byte to mouse\n");
      return -1;
    }
    kbc_read_out_buf(&local_output);
    if(local_output==MOUSE_ACK) break;
    else if(local_output==MOUSE_NACK) continue;
    else {
      printf("Error in acknowledge_byte\n");
      kbc_and_mouse_reset();
      return -1;
    }
  }
  while(1) {
    if(kbc_write_cmd(KBC_CMD_REG,MOUSE_WRITE_BYTE)!=0) {
      printf("Error asking register to write byte to mouse\n");
      return -1;
    }
    if(kbc_write_cmd(KBC_CMD_ARGS,MOUSE_DISABLE_DATA_REPORTING)!=0) {
      printf("Error writing byte to mouse\n");
      return -1;
    }
    kbc_read_out_buf(&local_output);
    if(local_output==MOUSE_ACK) break;
    else if(local_output==MOUSE_NACK) continue;
    else {
      printf("Error in acknowledge_byte\n");
      kbc_and_mouse_reset();
      return -1;
    }
  }*/
  kbc_write_cmd(KBC_CMD_REG,KBC_WRITE_COMBYTE);
  kbc_write_cmd(KBC_CMD_ARGS,minix_get_dflt_kbc_cmd_byte());
  return 0;
}

int unsubscribe_mouse(){

    if (sys_irqdisable(&mouse_hook_id) != OK) {
		return -1;
	}
	if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
		return -1;
	}
	return 0;
}



void (mouse_ih)() {
    uint32_t status;
        if (sys_inb(KBC_STATUS_REG, &status) != OK) {
            erro = 1;
        }
        //if data is available to be read
        if (status & KBC_OBF) {
        //if parity error, timeout error if it's mouse data instead of keyboard
        if ((status & (KBC_PARITY | KBC_TIMEOUT)) == 0) {
         if (sys_inb(KBC_OUT_BUF, &mouse_data)!=OK) {
              //error reading output buffer. retry?
              erro = 1;
            }

              erro = 0;
        }
        else {
         //error in parity,timeout. retry?
         erro = 1;
        }
    }
    //if it got here it means nothing was read. wait and try again
}

void struct_packet_filler(struct packet *pp) {
  if(pp->bytes[0] & RB) {
    pp->rb=true;
  }
  else pp->rb=false;

  if(pp->bytes[0] & LB) {
    pp->lb=true;
  }
  else pp->lb=false;

  if(pp->bytes[0] & MB) {
    pp->mb=true;
  }
  else pp->mb=false;

  if(pp->bytes[0] & Y_OVFL) {
    pp->y_ov=true;
  }
  else pp->y_ov=false;

  if(pp->bytes[0] & X_OVFL) {
    pp->x_ov=true;
  }
  else pp->x_ov=false;

    if(pp->bytes[0] & X_SIGN) {
    pp->delta_x= (uint32_t) (pp->bytes[1] | (-1 << 8));
  }
  else pp->delta_x=pp->bytes[1];

  if(pp->bytes[0] & Y_SIGN) {
    pp->delta_y= (uint32_t) (pp->bytes[2] | (-1 << 8));
  }
  else pp->delta_y=pp->bytes[2];

  return;
}




