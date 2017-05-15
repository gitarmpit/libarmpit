#include "cfg_model.h"

CfgModel::CfgModel(CFG* cfg)
{
	if (cfg)
	{
	    SetCfg(cfg);
	}
	//_cfgView = 0;
}

void CfgModel::SetCfg (CFG* cfg)
{
    _cfg = cfg;
    _pid[0].Init(_cfg->P, 0, 200, 1, 1);
    _pid[1].Init (_cfg->I, 0, 200, 1, 1);
    _pid[2].Init (_cfg->D, 0, 200, 1, 1);

}


//void CfgModel::SetView (CfgView* view)
//{
//	_cfgView = view;
//	_cfgView->Publish(_cfg);
//}


void CfgModel::IncrementLow(int8_t idx)
{
	if (idx < Count())
	{
		_pid[idx].IncrementLow();
		SetValue(idx);
	}
}

void CfgModel::DecrementLow(int8_t idx)
{
	if (idx < Count())
	{
		_pid[idx].DecrementLow();
		SetValue(idx);
	}
}

void CfgModel::IncrementHigh(int8_t idx)
{
	if (idx < Count())
	{
		_pid[idx].IncrementHigh();
		SetValue(idx);
	}
}

void CfgModel::DecrementHigh(int8_t idx)
{
	if (idx < Count())
	{
		_pid[idx].DecrementHigh();
		SetValue(idx);
	}
}


void CfgModel::SetValue (int8_t idx)
{
	float v;
	_pid[idx].GetValue(&v);

	if (idx == 0)
	{
		_cfg->P = v;
	}
	else if (idx == 1)
	{
		_cfg->I = v;
	}
	else if (idx == 2)
	{
		_cfg->D = v;
	}

//	if (_cfgView)
//	{
//		_cfgView->Publish(_cfg);
//	}

}




