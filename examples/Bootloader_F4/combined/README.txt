Bootloader and main app in the same project
TWO Flash sections in the linker script: 0x0 and the relocated application code. 
Se we  boot from 0x0 then change the vector table location and pass control to the main() 
in the program code located in a different sector



1. ldscript should define the relocated address:

_application_addr = 0x0800C000;

and two vector boot locations


this is a dummy vector that is placed at 0x0

  .isr_vector_boot :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector_boot)) /* Dummy startup code */
    . = ALIGN(4);
  } >BOOT


this is a relocated vector

  /* The relocated vector for the application starting at sector 3 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH




2. The start-up script will start from address 0x0 with a dummy vector table 

   .section  .isr_vector_boot,"a",%progbits
  .type  dummy_pfnVectors, %object
  .size  dummy_pfnVectors, .-dummy_pfnVectors

 dummy_pfnVectors:
  .word  _estack
  .word  Boot_Reset_Handler

 
 
Boot_Reset_Handler will redirect the vector table

  /* redirect interrupt vector table*/
  ldr r1, =0xE000ED08
  ldr r2, =_application_addr
  str r2, [r1]


then pass control to the actual Reset_Handler



The actual vector:

/* Relocated vector  */

   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
    
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler

