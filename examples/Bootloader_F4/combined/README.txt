Bootloader and main app in the same project
TWO Flash sections in the linker script: 0x0 and the relocated application code. 
Se we  boot from 0x0 then change the vector table location and pass control to the main() 
in the program code located in a different sector



Ldscript should define the relocated address:

_application_addr = 0x0800C000;

and set the FLASH to the new location: thats where program code will be loaded
notice there is also BOOT section in memory: that's the original 0 location, start of FLASH memory 
BOOT is only used to place the dummy vector there. 

MEMORY
{
BOOT (rx)      : ORIGIN = 0x08000000, LENGTH = 16K
FLASH (rx)      : ORIGIN = 0x0800C000, LENGTH = 976K  /* sector 3 */   <= This is where the program code will be loaded
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K
}


and two vector boot locations :

this is a dummy vector that is placed at 0x0, since thats the default location wheren the interrupt vector is expected

  .isr_vector_boot :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector_boot)) /* Dummy startup code */
    . = ALIGN(4);
  } >BOOT


this is a relocated vector, it goes to FLASH, beginning from sector 3 in our example, since thats where the program code will be load 

  /* The relocated vector for the application starting at sector 3 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH




The start-up script will start from address 0x0, since it is the BOOT section in the ld script
It contains a dummy vector table: 

  .section  .isr_vector_boot,"a",%progbits
  .type  dummy_pfnVectors, %object
  .size  dummy_pfnVectors, .-dummy_pfnVectors

 dummy_pfnVectors:
  .word  _estack
  .word  Boot_Reset_Handler

 
 
Boot_Reset_Handler will redirect the vector table

  /* redirect interrupt vector table*/
  ldr r1, =0xE000ED08
  ldr r2, =_application_addr  : FLASH sector 3 
  str r2, [r1]


then pass control to the actual Reset_Handler in FLASH sector 3


The actual vector:

/* Relocated vector  */

   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler


Reset_Handler  will do the init part and call main 

=========================
The whole point of moving the program code to sector 3 is to be able to persist program settings on FLASH 
Since FLASH sector #0 is our boot loader, we can persist to sector 1 or sector 2, 16k each, so we have 32k available in 2 sectors 

#define SECTOR1_OFFSET    0x08004000
#define SECTOR2_OFFSET    0x08008000
#define SECTOR_SIZE 16384

