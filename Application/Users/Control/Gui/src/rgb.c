#include "rgb.h"


uint16_t RGB_To_U16(uint32_t const Rgb) 
{
    return RGB_TO565(R_TO565(GET_H8(Rgb)), 
        G_TO565(GET_M8(Rgb)), B_TO565(GET_L8(Rgb)));
}
