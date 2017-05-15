    INCLUDE common.s
    INCLUDE imp.s
        

    AREA CODE, CODE, READONLY
        
    IMPORT ar
    IMPORT presc
    IMPORT clk_freq 
    IMPORT tim_ch
    
    
    EXPORT Capture_Cfg


    ALIGN 
    END