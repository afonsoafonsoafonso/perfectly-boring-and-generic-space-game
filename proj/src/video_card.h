// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

/** @defgroup video_card video_card
 * @{
 * 
 * 
 * 
 * 
 * 
 *  Functions responsible for video functionality
 */

#define VBE_SET_MODE 0x4F02
#define VBE_GET_MODE_INFO 0x4F01
#define VBE_GET_CTRL_INFO 0x4F00
#define VC_INT 0x10 
#define BIT(n) (0x01<<(n))
#define VBE_FCALL_SUPPORTED 0x4F //to check in AL register
#define VBE_FCALL_FAILED 0x01 //to check in AH register
#define VBE_FCALL_NOT_SUPPORTED_CURRENT_HW 0x02 //to check in AH register
#define VBE_FCALL_INVALID_CURRENT_VIDEOMODE 0x03 //to check in AH register
#define VBE_FCALL_SUCCESS 0x00 //to check in AH register

/**
 * @brief Gets info from specified mode
 * 
 * @param mode Mode from which we want to get info from
 * @param vmi_p Pointer to a struct where mode info will be stored
 * 
 * @return 0 uppon success, non zero otherwise
 */
int (get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Sets specified mode
 * 
 * @param mode Mode which we want to set 
 * 
 * @return 0 uppon success, non zero otherwise
 */
int (vc_set_mode)(uint16_t mode);

/**
 * @brief Maps needed vram for specified mode
 * 
 */
void *(vc_vram_mapping)(uint16_t mode);

/**
 * @brief Draws pixel on screen on given coordenates and with given color
 * 
 * @param x X pixel coordenate
 * @param y Y pixel coordenate
 * @param color Color which we want to set the pixel on
 * 
 * @return 0 if given coordenates are within resolution, non-zero otherwise
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Initializes minix in specified video mode
 * 
 */
void *(vg_init)(uint16_t mode); 

/**
 * @brief Updates main buffer with second buffer data
 */
void WriteToMainBuffer();

/**
 * @brief Allocates memory from auxiliar/second buffer
 * 
 */
void AllocateSecondBuffer();
