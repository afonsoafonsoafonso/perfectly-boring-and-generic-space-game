// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include <math.h>

//static global variables
static unsigned char *video_mem;
static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;
//macros to add to header file later
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

#define SEGMENT(x) ((x)  >> 16)

#define OFFSET(x) (x & 0xFFFF)

typedef uint32_t farpointer_t;

typedef struct __attribute__((packed)){ 

    char VbeSignature [4];

    BCD VbeVersion[2];

    farpointer_t OemStringPtr;

    uint32_t Capabilities;

    farpointer_t VideoModePtr;

    uint16_t         TotalMemory;

    BCD OemSoftwareRev [2];

    farpointer_t OemVendorNamePtr;

    farpointer_t OemProductNamePtr;

    farpointer_t OemProductRevPtr;

    uint8_t reserved [222];

    uint8_t OemData [256];

} vbe_auxiliar_struct;


int (get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p) {
    mmap_t buf; //é assim? só fui lá pela documentação e ajuda de outros colegas. mt pouco claro no guião/slides (https://web.fe.up.pt/~pfs/aulas/lcom2018/labs/lab5/src/doc/liblm_8h_source.html)
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

    //Mapping memory

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
        //if so, we calculate the pixel position
        uint32_t pixel_adress=(x + h_res * y) * (ceil(bits_per_pixel/8));
        memcpy(&video_mem[pixel_adress], &color, ceil(bits_per_pixel/8));
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

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for(int i=0; i<width; i++) {
        for(int j=0; j<height; j++) {
            draw_pixel(x+i, y+j, color);        
        }
    }
    return 0;
}

int (vg_draw_xpm)(const char *map[], int x, int y) {
    int width, height;
    char *xpm_adress=NULL;

    xpm_adress=read_xpm(map,&width,&height);

    if(xpm_adress==NULL) {
        printf("vg_draw_xpm :: read_xpm error\n");
        return -1;
    }

    printf("height:%d ; width:%d\n",height,width);

    for(int i=0; i<width; i++) {
        for(int j=0; j<height; j++) {
            if(draw_pixel(x+i,y+j,xpm_adress[j * width + i])!=0) {
                printf("vg_draw_xpm :: draw_pixel error\n");
                return -1;
            }
        }
    }

    free(xpm_adress);

    return 0;
}



int vbe_get_control_info(){

    mmap_t map;
    struct reg86u reg86;
    vg_vbe_contr_info_t control_info;
    vbe_auxiliar_struct aux;
    char string[4] = "VBE2";

    uint32_t* mem_base = lm_init(true);

    lm_alloc(sizeof(vbe_auxiliar_struct), &map);

    memcpy(map.virt, string, sizeof(string));

    memset(&reg86, 0, sizeof(reg86)); //zeroing the struct

    reg86.u.w.ax = VBE_GET_CTRL_INFO; //VBE call, function 00 : get control info
    reg86.u.w.es = PB2BASE(map.phys); /*set a segment base*/
    reg86.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */ 
    reg86.u.b.intno = VC_INT; //video card interrupt vector

   if( sys_int86(&reg86) != OK ) {
        printf("vbe_get_control_info :: sys_int86 failed\n");
        lm_free(&map);
        return -1;
    }

    //checking AL register acknowledgment
    if(reg86.u.b.al != VBE_FCALL_SUPPORTED) {
        printf("vbe_get_ctrl_info :: VBE function call not supported\n");
        return -1;
    }

    //checking AH register acknowledgment
    if(reg86.u.b.ah != VBE_FCALL_SUCCESS) {
        if(reg86.u.b.ah == VBE_FCALL_FAILED) {
            printf("vbe_get_ctrl_info :: VBE function call failed\n");
            return -1;
        }

        if(reg86.u.b.ah == VBE_FCALL_NOT_SUPPORTED_CURRENT_HW) {
            printf("vbe_get_ctrl_info :: VBE function call not supported in current HW configuration\n");
            return -1;
        }

        if(reg86.u.b.ah == VBE_FCALL_INVALID_CURRENT_VIDEOMODE) {
            printf("vbe_get_ctrl_info :: VBE function call invalid in current video mode\n");
            return -1;
        }
    }

    //Passing the information from the map to the struct
    memcpy(&aux, map.virt, map.size);

    //Freeing the map memory
    lm_free(&map);

    //Copying the required components
    strcpy(control_info.VBESignature, aux.VbeSignature);
    control_info.VBEVersion[0] = aux.VbeVersion[0];
    control_info.VBEVersion[1] = aux.VbeVersion[1];
    control_info.OEMString = (void*) ((uint32_t) mem_base + ((SEGMENT(aux.OemStringPtr)) << 4) + (OFFSET(aux.OemStringPtr)));

    //Turning 64kb blocks into kb
    control_info.TotalMemory = aux.TotalMemory * 64;
    control_info.VideoModeList = (void*) ((uint32_t) mem_base + ((SEGMENT(aux.VideoModePtr)) << 4) + (OFFSET(aux.VideoModePtr)));
    control_info.OEMVendorNamePtr = (void*) ((uint32_t) mem_base + ((SEGMENT(aux.OemVendorNamePtr)) << 4) + (OFFSET(aux.OemVendorNamePtr)));
    control_info.OEMProductNamePtr = (void*) ((uint32_t) mem_base + ((SEGMENT(aux.OemProductNamePtr)) << 4) + (OFFSET(aux.OemProductNamePtr)));
    control_info.OEMProductRevPtr = (void*) ((uint32_t) mem_base + ((SEGMENT(aux.OemProductRevPtr)) << 4) + (OFFSET(aux.OemProductRevPtr)));

    //Priting the struct
    vg_display_vbe_contr_info(&control_info);        
     return 0;
}

