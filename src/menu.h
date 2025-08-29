#ifndef __MENU_H__
#define __MENU_H__

#include <genesis.h>
#include "musicas.h"

#define NUM_PAUSE_OPTIONS 3
#define NUM_END_OPTIONS 3

// define the options structure
typedef struct 
{
    s16 x;
    s16 y;
    char text[32];
} Option;

typedef struct 
{
    s16 x;
    s16 y;
    char text[32];
    const Image* image;
    u8 difficulty;
} MusicOption;

const MusicOption music_options[NUM_MUSICAS];
const Option pause_options[NUM_END_OPTIONS];
const Option end_options[NUM_END_OPTIONS];

void show_initial_menu();
void show_pause_menu();

void show_menu(const Option *options, u8 size);

#endif