#ifndef _CFG_NUMBER_H
#define _CFG_NUMBER_H


class CfgNumber
{
private:
    int          _min;
    int          _max;
    int          _lowInc;
    int          _highInc;
    int          _value;
public:
    CfgNumber() {}
    void Init(int value, int min, int max, int lowInc, int HighInc);
    void Init(float value, int min, int max, int lowInc, int HighInc);

    void IncrementLow();
    void IncrementHigh();
    void DecrementLow();
    void DecrementHigh();

    void GetValue(int* v);
    void GetValue(float* v);


};



#endif
