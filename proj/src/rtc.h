/** @defgroup rtc rtc
 * @{
 * 
 * 
 * 
 *  Functions responsible for real-time clock functionality
 */

#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70	
#define RTC_DATA_REG 0x71	
#define SEC 0x00	
#define MINUTE 0x02	
#define HOUR 0x04	
#define DAY 0x07	
#define MONTH 0x08
#define YEAR 0x09	
#define REG_A 0x0A
#define REG_B 0x0B	
#define REG_C 0x0C
#define RTC_UIE BIT(4)
#define HOUR_FORMAT BIT(1)
#define REG_B_SET BIT(7)
#define REG_A_UIP BIT(7)

/**
 * @brief Subscribes rtc to interrupts
 * 
 * @return int Returns rtc bitmask if sucessful, -1 otherwise
 */
int rtc_subscribe();

/**
 * @brief Unsubscribes rtc to interrupts
 * 
 * @return int Returns 0 if sucessful, non-zero otherwise
 */
int rtc_unsubscribe();

/**
 * @brief Converts hexadecimal value into decimal value
 * 
 * @param x Hexadecimal value to be converted
 * @return int Result of conversion
 */
int convert_to_decimal(uint32_t x);

/**
 * @brief Initializes RTC
 * 
 * @return int Returns 0 if successful, non-0 otherwise
 */
int rtc_init();

/**
 * @brief Reads output from RTC
 * 
 * @param reg Register to be read from
 * @param data Pointer to variable where output from rtc will be stores
 * @return int Returns 0 if upon success, non-zero otherwise
 */
int rtc_read(uint32_t reg, uint32_t* data);

/**
 * @brief Reads and returns current seconds from RTC
 * 
 * @return int Current seconds
 */
int current_seconds();

/**
 * @brief Reads and returns current minutes from RTC
 * 
 * @return int Current minutes
 */
int current_minutes();

/**
 * @brief Reads and returns current hours from RTC
 * 
 * @return int Current hours
 */
int current_hours();

/**
 * @brief Reads and returns current day from RTC
 * 
 * @return int Current day
 */
int current_day();

/**
 * @brief Reads and returns current month from RTC
 * 
 * @return int Current month
 */
int current_month();

/**
 * @brief Reads and returns current year from RTC
 * 
 * @return int Current year
 */
int current_year();



