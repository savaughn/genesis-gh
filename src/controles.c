#include "controles.h"
#include "timer.h"

void control_initializeVariables()
{
    J1A = 0;
    J1B = 0;
    J1C = 0;
    J1S = 0;
    J1DOWN = 0;
    J1UP = 0;
    J1ACount = 0;
    J1BCount = 0;
    J1CCount = 0;
}

void control_Handler(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		// detect button A pressed
        if (changed & BUTTON_A)
        {
            if(state & BUTTON_A )
            {
                J1A = 1;
                J1ACount = getTick();
            }
            // button A released
            else
            {
                J1A = 0;
            }
        }
        // detect button B pressed
        if (changed & BUTTON_B)
        {
            if(state & BUTTON_B )
            {
                J1B = 1;
                J1BCount = getTick();
            }
            // button B released
            else
            {
                J1B = 0;
            }
        }
        // detect button C pressed
        if (changed & BUTTON_C)
        {
            if(state & BUTTON_C )
            {
                J1C = 1;
                J1CCount = getTick();
            }
            // button C released
            else
            {
                J1C = 0;
            }
        }
        if (changed & BUTTON_START)
        {
            if(state & BUTTON_START )
            {
                J1S = 1;
            }
            // button C released
            else
            {
                J1S = 0;
            }
        }

        if (changed & BUTTON_DOWN)
        {
            if(state & BUTTON_DOWN )
            {
                J1DOWN = 1;
            }
            else
            {
                J1DOWN = 0;
            }
        }
        if (changed & BUTTON_UP)
        {
            if(state & BUTTON_UP )
            {
                J1UP = 1;
            }
            else
            {
                J1UP = 0;
            }
        }
        
	}
}