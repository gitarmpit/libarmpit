#include <string.h>
#include "flash_cfg.h"

#define SECTOR1_OFFSET    0x08004000
#define SECTOR2_OFFSET    0x08008000
#define SECTOR_SIZE 16384

FlashCfg::FlashCfg()
{
    _current_entry = 0;
    _currentSector = 1;
    _cfg_entry_size = (sizeof(CFG) + 4);
    _max_cfg_entries = SECTOR_SIZE / _cfg_entry_size;
}

void FlashCfg::write()
{
    int32_t nwords = sizeof(CFG)/4;
    if (nwords * 4 != sizeof(CFG))
    {
        ++nwords;
    }

    int entry_offset = _current_entry * _cfg_entry_size;

    uint32_t sectorOffset = (_currentSector == 1) ? SECTOR1_OFFSET : SECTOR2_OFFSET;

    int status = FLASH_WriteWord(sectorOffset + entry_offset, FLASH_CFG_MAGIC_WORD);
    entry_offset += 4;

    uint32_t* pcfg = (uint32_t*)&_cfg;
    for (int32_t i = 0; i < nwords; ++i)
    {
        status = FLASH_WriteWord(sectorOffset + entry_offset + i*4, pcfg[i]);
    }

}

bool FlashCfg::save_cfg ()
{
    FLASH_Unlock();
    FLASH_SetPsize_x32();

    if (_current_entry >= _max_cfg_entries)
    {
        _current_entry = 0;

        int8_t sectorToErase = _currentSector;

        _currentSector = (_currentSector == 1) ? 2 : 1;

        write();

        if (0 != FLASH_EraseSector(sectorToErase))
        {
            return false;
        }
    }
    else
    {
        write();
    }

    FLASH_Lock();
    return true;
}

uint8_t* FlashCfg::ScanSector (uint32_t sectorOffset)
{
    int offset;
    int i = 0;
    uint8_t* p, *last = 0;
    for (; i < _max_cfg_entries; ++i)
    {
        offset = i * _cfg_entry_size;
        p = (uint8_t*)(sectorOffset + offset);
        if (*((uint32_t*)p) != FLASH_CFG_MAGIC_WORD)
        {
            break;
        }
        last = p;
    }

    _current_entry = i;

    return last;
}

CFG* FlashCfg::load_cfg ()
{
    _currentSector = 1;
    uint8_t *last = ScanSector(SECTOR1_OFFSET);

    //sector1 empty, try sector 2
    if (last == 0)
    {
        last = ScanSector(SECTOR2_OFFSET);
        if (last)
        {
            _currentSector = 2;
        }
    }

    //no cfg entries, fresh start
    if (last == 0)
    {
        memset (&_cfg, 0, sizeof(CFG));
        _cfg.P = 7;
        _cfg.I = 0.5;
        _cfg.D = 2.8;
    }
    else
    {
        memcpy(&_cfg, (last+4), sizeof(CFG));
    }

    return &_cfg;

}

//CFG* FlashCfg::get_cfg()
//{
//    return &_cfg;
//}


