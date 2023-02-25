#include <stdio.h> 
#include <stdint.h>


#define IS_BIT_SET(a,b)  (a & 1<<b) 

static float get_temp (int16_t t_raw, int len, int frac_len)
{
  float t = 0.0;
  t_raw >>= (16 - len);
  int int_len = len - frac_len - 1;

  float t_frac = 0.0;
  static char buf[256];

  for (int i = 0; i < frac_len; ++i)
  {
    if (t_raw & 1<<i)
    {
       t_frac += 1.0 / (1<<(frac_len-i));
    }
  }

  t_raw >>= frac_len;
  bool is_negative = false;

  if (IS_BIT_SET(t_raw, len - frac_len)) 
  {
    is_negative = true;
    t_raw = ~t_raw;
  }

  int t_int = 0; 
  for (int i = 0; i <= int_len; ++i)
  {
     if (t_raw & 1<<i)
     {
        t_int += (1<<i);
     }
  }

  
  if (is_negative) 
  {
    t_int = -t_int;    
  }

  t = t_int + t_frac;

  return t;

}

int main(void) 
{
    int i = 0x7;
    printf ("%d\n", IS_BIT_SET(i, 2));
    printf ("%d\n", IS_BIT_SET(i, 0));
    printf ("%d\n", IS_BIT_SET(i, 3));
}