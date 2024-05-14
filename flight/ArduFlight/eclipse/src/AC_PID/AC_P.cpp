// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/// @file	AC_P.cpp
/// @brief	Generic P algorithm

#include <AP_Math/AP_Math.h>
#include "AC_P.h"


float AC_P::get_p(float error) const
{
    return (float)error * _kp;
}

void AC_P::load_gains() //TODO
{
}

void AC_P::save_gains()
{
}
