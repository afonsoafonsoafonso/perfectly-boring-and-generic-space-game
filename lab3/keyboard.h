#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"
#define DELAY_US 20000
#define MAX_TRIES 10

#ifdef LAB3
    int sys_inb_cnt(port_t port, unsigned long* byte);
#else
    #define sys_inb_cnt(p,b) sys_inb(p,b)
#endif

/**
 * @brief Reads and returns status byte by reference
 *
 * @param Adress of memory to be filled with status byte
 * @return Return 0 upon sucess and non-zero otherwise
 */
int read_stat_reg(uint8_t *byte);

/** NOTE: we now this function would be more correct if it returned
 *  0 upon success and -1 otherwise and passed the hook_id by reference
 * just like we did on lab2. this way if, for some reason, the hook_id is -1
 * we don't know if the function worked or not. anyways it's too late to
 * change now but we'll keep this in mind in the future and will change it
 * accordingly
 *
 * @brief subscribes and enables keyboard interrupts
 *
 * @return Returns hook_id upon success and non-zero otherwise
 */
int keyboard_subscribe();

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int keyboard_unsubscribe();

/**
 * @brief writes any given command to any given register
 *
 * @param register to write to
 * @param command to write to register
 * @return Returns 0 upon success and non-zero otherwise
 */
int kbc_write_cmd(uint8_t reg, uint8_t cmd);

/**
 * @brief reads current data in output buffer
 *
 * @param memory adress to be filled with output buffer data
 * @return Returns 0 upon success and non-zero otherwise
 */
int kbc_read_out_buf(uint8_t *output);

/**
 * @brief analyzes and prints scancode
 *
 *
 * @param memory adress of scancode to be analyzed
 * @return no return (void)
*/
void print_scancode(uint8_t *kbc_scancode);
