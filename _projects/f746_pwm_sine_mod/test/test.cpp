#include <stdio.h>
#include <math.h>

int main(void)
{
    int total = 20000 / period;
    step = 180.0 / total * 2.0;
    int presc = 1;
    uint32_t t_clk = 216000000;
    for (int i = 0; i < total; ++i) {
      float s = 1.0 + sin (step*i*M_PI/180. - M_PI/2.0);
      float ds = _period*s/2.0;
      uint16_t ccr_val = (uint64_t)ds * (uint64_t)t_clk / 1000000ull / (uint64_t)(presc + 1);
      uint16_t ccr_val_ns = (uint64_t)((float)ds * 1000 * (float)t_clk / 1000000000.0 / (float)(presc + 1) + 0.5);
      printf ("%d %f %f %d %d\n", i, s, _table[i], ccr_val, ccr_val_ns);
      SysTick_Delay(1);
    }
}