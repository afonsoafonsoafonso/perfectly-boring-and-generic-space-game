#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "i8042.h"

/** @defgroup mouse mouse
 * @{
 * 
 * 
 * 
 *  Functions responsible for mouse functionality
 */

/**
 * @brief Resets the kbc and mouse to Minix's default settings
 * 
 */
void kbc_and_mouse_reset();

/**
 * @brief Parses the packets from the mouse and fills the struct packet with the parsed mouse information
 * 
 * @param Struct to be filled (altough the array with the packets must be already filled in and ready to be parsed)
 */
void struct_packet_filler(struct packet *pp);

int mouse_read_out_buf(uint32_t *output);

/**
 * @brief Writes byte to mouse
 * 
 * @param cmd Command to be writen to mouse
 * @return int Returns 0 if command given was successfuly writen and non zero otherwise
 */
int mouse_write_byte(uint8_t cmd);

/**
 * @brief Subscribes mouse to interrupts
 * 
 * @return int Returns mouse bitmask if successful, -1 otherwise
 */
int subscribe_mouse();

/**
 * @brief Enables mouse notifications
 * 
 * @return int Returns 0 if successful, non-zero otherwise
 */
int mouse_enable_int();

/**
 * @brief Disables mouse notifications
 * 
 * @return int Returns 0 if successful, non-zero otherwise
 */
int mouse_disable_int();

/**
 * @brief Unsubscribes mouse to interrupts
 * 
 * @return int Returns 0 if successful, non-zero otherwise
 */
int unsubscribe_mouse();
