// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include <math.h>
#include "video_card.h"

//static global variables
static unsigned char *video_mem;
static unsigned char *second_mem;
static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;

int (get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p) {
    mmap_t buf;
    struct reg86u r;

    memset(&r, 0, sizeof(r)); //zeroing the struct
    
    lm_init(true);

    lm_alloc(sizeof(vbe_mode_info_t),&buf);

    r.u.w.ax=VBE_GET_MODE_INFO; /*vbe get mode info*/
    /*translate the buffer linear adress to a far pointer*/
    r.u.w.es=PB2BASE(buf.phys); /*set a segment base*/
    r.u.w.di=PB2OFF(buf.phys); /*set the offset accordingly*/
    r.u.w.cx=mode;
    r.u.b.intno=VC_INT;
    if( sys_int86(&r) != OK ) {
        printf("vbe_get_mode_info :: sys_int86 failed\n");
        lm_free(&buf);
        return -1;
    }
    //checking AL register acknowledgment
    if(r.u.b.al != VBE_FCALL_SUPPORTED) {
        printf("vc_set_mode :: VBE function call not supported\n");
        return -1;
    }
    //checking AH register acknowledgment
    if(r.u.b.ah != VBE_FCALL_SUCCESS) {
        if(r.u.b.ah == VBE_FCALL_FAILED) {
            printf("vc_set_mode :: VBE function call failed\n");
            return -1;
        }
        if(r.u.b.ah == VBE_FCALL_NOT_SUPPORTED_CURRENT_HW) {
            printf("vc_set_mode :: VBE function call not supported in current HW configuration\n");
            return -1;
        }
        if(r.u.b.ah == VBE_FCALL_INVALID_CURRENT_VIDEOMODE) {
            printf("vc_set_mode :: VBE function call invalid in current video mode\n");
            return -1;
        }
    }

    memcpy(vmi_p, buf.virt, sizeof(vbe_mode_info_t));

    lm_free(&buf);

    return 0;
}

int (vc_set_mode)(uint16_t mode) {
//TODO: implement this function and vc_vram_mapping in same function
    struct reg86u reg86;

    memset(&reg86, 0, sizeof(reg86)); //zeroing the struct

    reg86.u.w.ax = VBE_SET_MODE; //VBE call, function 02 : set VBE mode
    reg86.u.w.bx = BIT(14) | mode; //setting mode but with BIT#14 set (linear framebuffer)
    reg86.u.b.intno = VC_INT; //video card interrupt vector

    if(sys_int86(&reg86) != OK) {
        printf("vc_set_mode :: Sys86 error\n");
        return -1;
    }

    //checking AL register acknowledgment
    if(reg86.u.b.al != VBE_FCALL_SUPPORTED) {
        printf("vc_set_mode :: VBE function call not supported\n");
        return -1;
    }
    //checking AH register acknowledgment
    if(reg86.u.b.ah != VBE_FCALL_SUCCESS) {
        if(reg86.u.b.ah == VBE_FCALL_FAILED) {
            printf("vc_set_mode :: VBE function call failed\n");
            return -1;
        }
        if(reg86.u.b.ah == VBE_FCALL_NOT_SUPPORTED_CURRENT_HW) {
            printf("vc_set_mode :: VBE function call not supported in current HW configuration\n");
            return -1;
        }
        if(reg86.u.b.ah == VBE_FCALL_INVALID_CURRENT_VIDEOMODE) {
            printf("vc_set_mode :: VBE function call invalid in current video mode\n");
            return -1;
        }
    }
    //if it got here, specifed mode was set successfuly
    return 0;
}

void *(vc_vram_mapping)(uint16_t mode) {
    //getting vbe mode info

    vbe_mode_info_t vbe_mode_info;
    if(get_mode_info(mode,&vbe_mode_info)!=0) {
        printf("vc_vram_mapping :: error in vbe_get_mode_info\n");
        return NULL;
    }
    //loading vbe mode info in static global variables
    h_res=vbe_mode_info.XResolution;
    v_res=vbe_mode_info.YResolution;
    bits_per_pixel=vbe_mode_info.BitsPerPixel;
    printf("h_res:%u ; v_res:%u ; bits_per_pixel:%u\n", h_res, v_res, bits_per_pixel);
    //calculating total memoery to be mapped : v_pixels * h_pixels * bytes/pixel
    uint32_t vram_size = h_res * v_res * (bits_per_pixel/8);

    //Allow memory mapping

    unsigned int vram_base=vbe_mode_info.PhysBasePtr;
    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    printf("base:%u ; limit:%u ; size:%u\n", mr.mr_base, mr.mr_limit, vram_size);

    int r;
    if( (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return NULL;
    }

    //Mapping memory and allocating space for second buffer

    second_mem = (unsigned char*) malloc(vram_size);
    if(second_mem == NULL) {
        printf("Error in alocating memory for second buffer.\n");
    }
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return NULL;
    }
    return video_mem;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    //checking if given coordenates are within resolution
    if(x>=0 && x<h_res && y>=0 && y<v_res) {
        if(color == CHROMA_KEY_GREEN_565) {
            return 0;
        }
        //if so, we calculate the pixel position
        uint32_t pixel_adress=(x + h_res * y) * (ceil(bits_per_pixel/8));
        memcpy(&second_mem[pixel_adress], &color, ceil(bits_per_pixel/8));
        return 0;
    }
    else return -1;
}

void *(vg_init)(uint16_t mode) {
    if(vc_vram_mapping(mode)==NULL) {
        printf("video_test_init :: vc_vram_mapping failed\n");
        return NULL;
    }

    if(vc_set_mode(mode)!=0) {
        printf("video_test_int :: vc_set_mode failed\n");
        return NULL;
    }
    
    return video_mem;
}

void WriteToMainBuffer() {
    memcpy(video_mem,second_mem,h_res*v_res*ceil(bits_per_pixel/8));
    return;
}
