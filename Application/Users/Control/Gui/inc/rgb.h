#ifndef __RGB_H__
#define __RGB_H__

#include "stm32f4xx.h"


#define GET_H8(v) (v>>16)
#define GET_M8(v) ((v&0xFFFF)>>8)
#define GET_L8(v) (v&0xFF)

#define R_TO565(r) (31*r/255)
#define G_TO565(g) (63*g/255)
#define B_TO565(b) (31*b/255)

#define RGB_TO565(r,g,b) ((r)|(g<<5)|(b<<11))

#define COLOR16(v) RGB_To_U16(v)


uint16_t RGB_To_U16(uint32_t const Rgb);

#endif
