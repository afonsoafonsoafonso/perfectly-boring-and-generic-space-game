#include "i8042.h"
#include <machine/asm.h>

.global _C_LABEL(kbc_asm_ih)

.data


.text
_C_LABEL(kbc_asm_ih):
    inb $KBC_STATUS_REG, %al

    //test %al, (PARITY | TIME_OUT | SYS_FLAG)
    //jz erro

    inb $KBC_OUT_BUF, %al
    movb %al, _C_LABEL(scancode)
    //movb 0, %dl
    //movb %dl, _C_LABEL(asm_erro)
    jmp end

//erro:
    //movb 1, %dl
    //movb %dl, _C_LABEL(asm_erro)
    //jmp end

end:
    ret
