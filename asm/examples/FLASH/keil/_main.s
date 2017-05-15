    PRESERVE8
    THUMB

    INCLUDE stm32.s
 
    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF

    AREA    CODE, CODE, READONLY

    IMPORT FLASH_SetWaitState    
    IMPORT FLASH_GetWaitState    
    IMPORT FLASH_Unlock
    IMPORT FLASH_Write   
    IMPORT FLASH_ErasePage
    IMPORT FLASH_Lock

    EXPORT   __main
__main PROC 
 
    
    bl FLASH_Unlock
    bl FLASH_Lock
    bl FLASH_Unlock

 
    ;mov r0, #2
    ;bl FLASH_SetWaitState
    ;bl FLASH_GetWaitState
 
    mov r0, #63
    bl FLASH_ErasePage
 
    ldr r0, =0x800FC00
    ldr r1, =0x1234
    bl FLASH_Write

    mov r0, #63
    bl FLASH_ErasePage

l   b l

    
    ENDP

    ALIGN

    END