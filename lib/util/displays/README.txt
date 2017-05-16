Nokia 5110  SPI framebuffer
command : dc ss transmit SPI 
data :   same
setContrast : SPI command

display:  sends buffer via SPI  

virtuals from GFX:
drawPixel:  updates buffer

================================================================
128x128
9163  SPI  framebuffer

      low level SPI 
    void writecommand(uint8_t c);
    void writedata(uint8_t d);
    void writedata16(uint16_t d);

drawPixel:  updates buffer

fillScreen:  update buffer 
clearScreen:


display: setAddrWindow + send buffer array over SPI 


homeAddress: setAddrWindow to 0,0 

setRotation: colorSpace(set mactrl) + writeCommand(MADCTL) + writeData(mactrl)


scroll:  sends SPI cmd 

display(on/off) : sends CMD_DISPON/OFF

======================================
9340/9341 SPI  240x320

similar to 9163  code not optimized
=====================================
SSD 1306    I2c  framebuffer 

drawPixel:  updateFrameBuffer
display: send buffer over I2C
invertDisplay: send cmd 

draw fast lines  its own implementation

======================================

