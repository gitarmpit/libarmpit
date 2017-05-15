            IF :DEF:STM32F10X_LD
            INCLUDE startup_stm32f10x_ld.s
            ELIF :DEF:STM32F10X_LD_VL
            INCLUDE startup_stm32f10x_ld_vl.s
            ELIF :DEF:STM32F10X_MD
            INCLUDE startup_stm32f10x_md.s
            ELIF :DEF:STM32F10X_MD_VL
            INCLUDE startup_stm32f10x_md_vl.s
            ELIF :DEF:STM32F10X_HD
            INCLUDE startup_stm32f10x_hd.s
            ELIF :DEF:STM32F10X_HD_VL
            INCLUDE startup_stm32f10x_hd_vl.s
            ELIF :DEF:STM32F10X_XL
            INCLUDE startup_stm32f10x_xl.s
            ELIF :DEF:STM32F10X_CL
            INCLUDE startup_stm32f10x_cl.s
            ELSE
            ASSERT FALSE
            ENDIF   
            
            END