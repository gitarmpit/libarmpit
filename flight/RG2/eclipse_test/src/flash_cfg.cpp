#include "flash_cfg.h"
#include <string.h>


void flash_cfg()
{
	FlashCfg* flashCfg = FlashCfg::GetInstance();
	CFG* cfg = flashCfg->load_cfg();

	cfg->D = 1.0;
    cfg->I = 2.0;
    cfg->P = 3.0;


    flashCfg->save_cfg();
}

