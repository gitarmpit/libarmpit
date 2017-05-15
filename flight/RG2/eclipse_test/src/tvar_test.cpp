//int tvar(char *buf, const char *fmt, ...)
//{
//  va_list args;
//  va_start(args, buf);
//  volatile uint32_t d1 = va_arg(args, uint32_t);
//  volatile uint32_t d2 = va_arg(args, uint32_t);
//  volatile uint64_t d = ((uint64_t)d2<<32) | (uint64_t)d1;
//  double dd = *(double*)&d;
//  //int n = ee_vsprintf(buf, fmt, args);
//  va_end(args);
//  //return n;
//  return 0;
//}

