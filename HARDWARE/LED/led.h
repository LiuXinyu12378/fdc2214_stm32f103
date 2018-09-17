#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define led PEout(2)// PE.2
#define key1 PEin(3)// PE.3
#define key2 PEin(4)// PE.4
#define key3 PEin(5)// PE.5
#define key4 PEin(6)// PE.6
#define key5 PFin(1)// PE.5
#define key6 PFin(2)// PE.6
#define button PFin(0)
void LED_Init(void);//≥ı ºªØ
void Key_Init(void);		    
#endif
