#define BIT(n) (0x01<<(n))

#define KBC_IRQ 1 // IRQ line

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

#define KBC_CMD_REG 0x64
#define KBC_READ_COMBYTE 0x20
#define KBC_WRITE_COMBYTE 0x60
#define KBC_CHECK 0xAA // returns 0x55 if ok or 0xFC if not
#define KBC_CHECK_IF 0xAB // returns 0 if ok
#define KBC_DISABLE_IF 0xAD
#define KBC_ENABLE_IF 0xAE

#define KBC_CB_INT BIT(0)   // enable interrupt on OBF from keyboard
#define KBC_CB_INT2 BIT(1)  // enable interrupt on OBF from mouse
#define KBC_CB_DIS BIT(4)   // diable keyboard interface
#define KBC_CB_DIS2 BIT(5)  // disable mouse
