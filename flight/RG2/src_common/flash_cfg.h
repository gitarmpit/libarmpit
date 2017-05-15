#ifndef _FLASH_CFG_H
#define _FLASH_CFG_H

#include <stdint.h>
#include "flash.h"
#include "cfg.h"

#define FLASH_CFG_MAGIC_WORD 0xEBEDBABA


class FlashCfg
{
private:
    CFG _cfg;
    int32_t _current_entry;
    int32_t _currentSector;
    int32_t _cfg_entry_size;
    int32_t _max_cfg_entries;

    FlashCfg(const FlashCfg&);
    FlashCfg();
    void operator=(FlashCfg const&);

    uint8_t* ScanSector (uint32_t sectorOffset);
    void write();

public:
    static FlashCfg* GetInstance()
    {
        static FlashCfg instance;
        return &instance;
    }

    bool save_cfg ();
    CFG* load_cfg ();
//    CFG* get_cfg();

};

#endif
