

#ifdef __cplusplus
extern "C"
{
#endif


//In all the examples system init is done as the first step in main (starting clocks, setting flash latency etc) 
//Can also do it here. (Makes no difference?)  This location is pointed to by the assembly start-up files provided by ARM
void SystemInit(void)
{
}


#ifdef __cplusplus
}
#endif
