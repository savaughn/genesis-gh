#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include <genesis.h>


extern bool J1A, J1B, J1C, J1S, J1DOWN, J1UP;
extern u16 J1ACount, J1BCount, J1CCount;

void control_initializeVariables();
void control_Handler(u16 joy, u16 changed, u16 state);
#endif