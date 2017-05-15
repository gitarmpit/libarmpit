    PRESERVE8
    THUMB

    INCLUDE stm32.s
    INCLUDE macros.s
            
    AREA DR, DATA, READONLY 
    INCLUDE spi_regs.s
            
SPI_ports  
    DCD     SPI1_BASE, 
    DCD     SPI2_BASE
    DCD     SPI3_BASE
      
            
    AREA CODE, CODE, READONLY 

    EXPORT SPI_Init 
            
            
SPI_Init PROC
  
    bx lr
    ENDP
        
        
            
    END 