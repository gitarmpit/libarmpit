#ifdef STM32F10X_LD
#include  startup_stm32f10x_ld.s
#ifdef STM32F10X_LD_VL
#include  startup_stm32f10x_ld_vl.s
#ifdef STM32F10X_MD
#include  startup_stm32f10x_md.s
#ifdef STM32F10X_MD_VL
#include  startup_stm32f10x_md_vl.s
#ifdef STM32F10X_HD
#include  startup_stm32f10x_hd.s
#ifdef STM32F10X_HD_VL
#include  startup_stm32f10x_hd_vl.s
#ifdef STM32F10X_XL
#include  startup_stm32f10x_xl.s
#ifdef STM32F10X_CL
#include  startup_stm32f10x_cl.s
#else error "chip type not defined"
#endif