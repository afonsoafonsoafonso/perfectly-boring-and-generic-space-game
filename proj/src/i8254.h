#pragma once

/** @defgroup i8254 i8254
 * @{
 * 
 * 
 * 
 *  i8254 controller macros
 */

#define TIMER_FREQ  1193182    /**< @brief clock frequency for timer in PC and AT */

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0			0x40 /**< @brief Timer 0 count register */
#define TIMER_1			0x41 /**< @brief Timer 1 count register */
#define TIMER_2			0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL		0x43 /**< @brief Control register */

#define SPEAKER_CTRL		0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0		0x00     /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1		BIT(6)   /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2		BIT(7)   /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD		(BIT(7)|BIT(6))  /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB		BIT(4)  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB		BIT(5)  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB		(TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE		(BIT(2)|BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN		BIT(2)          /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD			0x01   /**< @brief Count in BCD */
#define TIMER_BIN			0x00   /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_         BIT(5)
#define TIMER_RB_STATUS_        BIT(4)
#define TIMER_RB_SEL(n)         BIT((n)+1)

//Status Byte macros (timer_display_conf specific)
#define STBT_OUT_LOW CHECK_BIT(st,7)==0
#define STBT_OUT_HIGH CHECK_BIT(st,7)!=0

#define STBT_INIT_LSB ((st & (BIT(5) | BIT(4))) >> 4)==1
#define STBT_INIT_MSB ((st & (BIT(5) | BIT(4))) >> 4)==2
#define STBT_INIT_LSB_MSB ((st & (BIT(5) | BIT(4))) >> 4)==3

#define STBT_MODE ((st & (BIT(3) | BIT(2) | BIT(1))) >> 1)

#define STBT_BASE_BCD CHECK_BIT(st,0)!=0
#define STBT_BASE_BIN CHECK_BIT(st,0)==0

//Other grup added Constants/Macros
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
