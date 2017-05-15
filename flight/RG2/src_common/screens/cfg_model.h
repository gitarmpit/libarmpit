#ifndef _CFG_MODEL_H
#define _CFG_MODEL_H
#include "flash_cfg.h"
#include "cfg_number.h"
#include "cfg.h"


class CfgModel
{
private:
	CFG*      _cfg;
	CfgNumber _pid[3];
	//CfgView*  _cfgView;

	void SetValue (int8_t idx);
public:

	CfgModel (CFG* cfg = 0);

	CFG* GetCfg() { return _cfg; }

	void SetCfg (CFG* cfg);

	//void SetView (CfgView* view);

    void IncrementLow(int8_t idx);
    void IncrementHigh(int8_t idx);
    void DecrementLow(int8_t idx);
    void DecrementHigh(int8_t idx);


    uint8_t Count() { return 3; }

};



#endif
