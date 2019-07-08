// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "rtc.h"
#include "i8042.h"
#include "game_structs.h"

static int rtc_hook_id = 8;

int rtc_subscribe(){

    int bitmask = BIT(rtc_hook_id);

    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id)!= OK) {
	  return -1;
    }
    
    
    return bitmask;
}

int rtc_unsubscribe(){

    if (sys_irqdisable(&rtc_hook_id) != OK) {
		return -1;
	}
    if (sys_irqrmpolicy(&rtc_hook_id) != OK) {
		return -1;
	}

	return 0;
}

int convert_to_decimal(uint32_t x){
    return (((x & 0xF0) >> 4) * 10) + (x & 0x0F);
}

int rtc_read(uint32_t reg, uint32_t* data){

    //Asking to read register "reg"
    if(sys_outb(RTC_ADDR_REG, reg)!= OK){
        return -1;
    }
    //Reading register "reg"
    if(sys_inb(RTC_DATA_REG, data)!= OK){
        return -1;
    }
    return 0;
}

int wait_rtc() {

  uint32_t regA = 0;

  sys_irqdisable(&rtc_hook_id);

  while(regA & REG_A_UIP){

    if(rtc_read(REG_A, &regA) == -1){
        return -1;
    }
  }

  sys_irqenable(&rtc_hook_id);
  return 0;
}


int rtc_init(){
    uint32_t regB = 0;

    if(rtc_read(REG_B, &regB) == -1){
        return -1;
    }
    
    //Enabling periodic interruptions by setting UIE bit
    regB|= RTC_UIE;

    //If set, inhibts updates of time/date registers
    regB &= ~REG_B_SET;

    //Setting 24h format
    regB|= HOUR_FORMAT;

    if(sys_outb(RTC_ADDR_REG, REG_B) != OK) {
		return -1;
    }

	if(sys_outb(RTC_DATA_REG, regB) != OK) {
		return -1;
    }
    return 0;
}

int current_seconds(){
	uint32_t sec = 0;
    wait_rtc();
	if(rtc_read(SEC, &sec) == -1){
        return -1;
    }
	return convert_to_decimal(sec);
}

int current_minutes(){
	uint32_t min = 0;
    wait_rtc();
	if(rtc_read(MINUTE, &min) == -1){
        return -1;
    }
	return convert_to_decimal(min);
}

int current_hours(){
	uint32_t hours = 0;
    wait_rtc();
	if(rtc_read(HOUR, &hours) == -1){
        return -1;
    }
	return convert_to_decimal(hours);
}

int current_day(){
	uint32_t day = 0;
    wait_rtc();
	if(rtc_read(DAY, &day) == -1){
        return -1;
    }
	return convert_to_decimal(day);
}

int current_month(){
	uint32_t month = 0;
    wait_rtc();
	if(rtc_read(MONTH, &month) == -1){
        return -1;
    }
	return convert_to_decimal(month);
}

int current_year(){
	uint32_t year = 0;
    wait_rtc();
	if(rtc_read(YEAR, &year) == -1){
        return -1;
    }
	return convert_to_decimal(year);
}



