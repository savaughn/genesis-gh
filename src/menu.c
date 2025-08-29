#include "menu.h"
#include "string.h"
#include "bg.h"

#define CENTER_X 20
#define CENTER_Y 14

const Option pause_options[NUM_PAUSE_OPTIONS] = {
    {CENTER_X-2, CENTER_Y, "RESUME"},
    {CENTER_X-2, CENTER_Y+1, "EXIT"},
    {CENTER_X-2, CENTER_Y+2, "RESTART"},
};

const MusicOption music_options[TRACK_COUNT] = {
    {CENTER_X-2, CENTER_Y-4, "Green Hill Zone", &sonic_cover, 3},
    {CENTER_X-1, CENTER_Y-4, "Back in Black",  &sonic_cover, 1},
    {CENTER_X-2, CENTER_Y-4, "Streets of Rage",  &sor_conver, 2},
    {CENTER_X, CENTER_Y-4, "Guile theme",  &guile_cover, 2},
    {CENTER_X-2, CENTER_Y-4, "Zelda-overworld",  &zelda_cover, 3},
    {CENTER_X-2, CENTER_Y-4, "Dracula's castle",  &sont_conver, 2},
    {CENTER_X-2, CENTER_Y-4, "Top Gear",  &topgear_cover, 3},
    {CENTER_X-2, CENTER_Y-4, "Altered Beast",  &alterd_cover, 1},
    {CENTER_X-2, CENTER_Y-4, "Smooth Criminal",  &moonwalker_conver, 1},
    {CENTER_X-2, CENTER_Y-4, "Motavia Town",  &phantasy_cover, 1},
};

const Option initial_menu_option =  {CENTER_X -8, CENTER_Y, "Press Start"};

const Option end_options[NUM_END_OPTIONS] = {
    {CENTER_X-2, CENTER_Y, "EXIT"},
    {CENTER_X-2, CENTER_Y+1, "RESTART"},
};

void show_initial_menu()
{
    VDP_drawText(initial_menu_option.text, initial_menu_option.x, initial_menu_option.y);
}

void show_pause_menu()
{
    for(u8 i = 0; i < NUM_PAUSE_OPTIONS; i++)
    {
        VDP_drawText(pause_options[i].text, pause_options[i].x, pause_options[i].y);
    }
}

void show_menu(const Option *options, u8 size)
{
    for(u8 i = 0; i < size; i++)
    {
        VDP_drawText(options[i].text, options[i].x, options[i].y);
    }
}