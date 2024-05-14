#include "AP_Common.h"

/*
  Return true if value is between lower and upper bound inclusive.
  False otherwise.
*/
bool is_bounded_int32(int32_t value, int32_t lower_bound, int32_t upper_bound)
{
    if ((lower_bound <= upper_bound) &&
        (value >= lower_bound) && (value <= upper_bound)) {
        return true;
    }

    return false;

}
