#pragma once

/** @defgroup i8042 i8042
 * @{
 * 
 * 
 *  i8042 controller macros
 */

#define BIT(n) (0x01<<(n))

#define KBC_IRQ 1 // IRQ line
#define MOUSE_IRQ 12 // IRQ line

#define DELAY_US    20000
#define MAX_TRIES 10

#define KBC_STATUS_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_CMD_ARGS 0x60
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60

#define KBC_OBF BIT(0)       //set if output buffer full and has data available to be read. goes *****************************//back to 0 if it is read
#define KBC_IBF BIT(1)       //if set, input buffer if full (dont write commands/arguments)
#define KBC_SYS BIT(2)       //run by controller's command byte. Goes to 0 after a power on reset
#define KBC_A2 BIT(3)        //if not set writes in data; if set writes in command
#define KBC_INH BIT(4)       //not set if keyboard is inhibited
#define KBC_AUX BIT(5)       //set if mouse data instead of keyboard
#define KBC_TIMEOUT BIT(6)   //set if timeout error (invalid data)
#define KBC_PARITY BIT(7)    //set if parity error (invalid data)

#define ESC_MAKE  0x01
#define ESC_BREAK 0x81
#define SPACE_MAKE 0x39

#define KBC_CMD_REG 0x64
#define KBC_READ_COMBYTE 0x20
#define KBC_WRITE_COMBYTE 0x60
#define KBC_CHECK 0xAA // returns 0x55 if ok or 0xFC if not
#define KBC_CHECK_IF 0xAB // returns 0 if ok
#define KBC_DISABLE_IF 0xAD
#define KBC_ENABLE_IF 0xAE

#define MOUSE_WRITE_BYTE 0xD4

#define KBC_CB_INT BIT(0)   // enable interrupt on OBF from keyboard
#define KBC_CB_INT2 BIT(1)  // enable interrupt on OBF from mouse
#define KBC_CB_DIS BIT(4)   // diable keyboard interface
#define KBC_CB_DIS2 BIT(5)  // disable mouse

#define MOUSE_RESET 0xFF /** Resets the mouse */
#define MOUSE_SET_DEFAULTS 0xF6 /** Sets default values */
#define MOUSE_DISABLE_DATA_REPORTING 0xF5 /** Disables data reporting. Should be sent before any command in stream mode */
#define MOUSE_ENABLE_DATA_REPORTING 0xF4 /** Enables data reporting, stream mode only */
#define MOUSE_SET_SAMPLE_RATE 0xF3 /** Sets state sampling rate  */
#define MOUSE_SET_REMOTE_MODE 0xF0 /** Sets remote mode, sending data on request only */
#define MOUSE_READ_DATA 0xEB /** Sends data packet request */
#define MOUSE_SET_STREAM_MODE 0xEA /** Sets stream mode, sends data on mouse events */
#define MOUSE_STATUS_REQUEST 0xE9 /** Gets mouse configuration (3 bytes) */
#define MOUSE_SET_RESOLUTION 0xE8 /** Sets mouse resolution */
#define MOUSE_SET_SCALING21 0xE7 /** Sets scaling to 2:1, Acceleration Mode */
#define MOUSE_SET_SCALING11 0xE6 /** Sets scaling to 1:1, Linear Mode */

#define Y_OVFL BIT(7)
#define X_OVFL BIT(6)
#define Y_SIGN BIT(5)
#define X_SIGN BIT(4)
#define MB BIT(2)
#define RB BIT(1)
#define LB BIT(0)

#define MOUSE_ACK 0xFA /** Acknowledged, everything is OK */
#define MOUSE_NACK 0xFE /** Not Acknowledged, invalid byte, might be because of a serial communication error, resend */
#define MOUSE_ERROR 0xFC /** Second consecutive invalid byte */

