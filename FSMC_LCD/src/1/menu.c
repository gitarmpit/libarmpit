
#define BlockSize            512 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define Bank1_SRAM_ADDR      0x68000000
#define Bank1_NOR_ADDR       0x64000000
#define LCD_RAM_ADDR         0x6C000002
#define Bank1_NAND_ADDR      0x70000000
#define START_ADDR           0x08019000
#define FLASH_PAGE_SIZE      0x800

#define NAND_OK   0
#define NAND_FAIL 1
#define MAX_PHY_BLOCKS_PER_ZONE  1024
#define MAX_LOG_BLOCKS_PER_ZONE  1000

extern __IO uint32_t DMAComplete;
extern __IO uint32_t FirstEntry;

uint32_t Buffer_Block_Rx[BufferWordsSize * 20];
uint8_t pBuffer[16384], TxBuffer[256];
__IO uint32_t DMASourceAddress = 0;
__IO uint32_t fractionaltimeunits = 0;



void InternalFlashToLCD(void)
{
    LCD_Clear(White);
    LCD_WriteBMP(START_ADDR);
    LCD_WriteBMP((START_ADDR + 0x25842));

}

void InternalFlashToLCD_DMA(void)
{

  while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8) != RESET)
  {
    LCD_WriteDMA(START_ADDR);
    LCD_WriteDMA((START_ADDR + 0x25842));
  }

} 

void LCD_WriteDMA(uint32_t address)
{
  DMA_InitTypeDef  DMA_InitStructure;

  LCD_WriteReg(R3, 0x1008);
 
  LCD_WriteRAM_Prepare();
  
  DMAComplete = 0;
  FirstEntry = 1;
  DMASourceAddress = address + 0x42;
  
  /* DMA1 channel3 configuration */
  DMA_DeInit(DMA1_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DMASourceAddress;  
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)LCD_RAM_ADDR;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 0xFFFF;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
  
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

  /* Enable DMA Channel3 Transfer Complete interrupt */
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

  /* Enable DMA1 channel3 */
  DMA_Cmd(DMA1_Channel3, ENABLE);

  /* Wait for DMA transfer Complete */
  while(DMAComplete == 0)
  {
  }
}

