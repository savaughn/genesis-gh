#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <genesis.h>

#define RED_X 231
#define RED_X_START FIX16(160.0)
#define GREEN_X 142
#define YELLOW_X 49
#define YELLOW_X_START FIX16(122.0)

#define RED_B_X RED_X +17
#define RED_B_X_START RED_X_START + FIX16(17)
#define GREEN_B_X GREEN_X +17
#define YELLOW_B_X YELLOW_X + 16
#define YELLOW_B_X_START YELLOW_X_START + FIX16(17)


#define SONIC_SIZE 434

typedef enum // Create an ENUM to identify a note
{
	YELLOW = 1,
	GREEN = 2,
    RED = 4
} Note; // Define a note

typedef enum // Create an ENUM to identify a music
{
	SONIC,
	BACK_IN_BLACK,
	STREETS_OF_RAGE,
	GUILE_THEME,
	ZELDA,
	DRACULA,
	TOP_GEAR,
	ALTERED,
	SMOOTH,
	PHATASY,
	NUM_MUSICAS,
} Music; // Define a music

const u32 sonic_times[434];
const Note sonic_notes[434];
const u16 sonic_duration[434];
const u16 sonic_size;
const u8 sonic_velocity;
const u16 sonic_delay;
const u8 sonic_loops;


const Note bib_notes[223];
const u16 bib_duration[223];
const u32 bib_times[223];
const u16 bib_size;
const u8 bib_velocity;
const u16 bib_delay;
const u8 bib_loops;

const Note sor_notes[165];
const u16 sor_duration[165];
const u32 sor_times[165];
const u16 sor_size;
const u8 sor_velocity;
const u16 sor_delay;
const u8 sor_loops;

const Note guile_notes[259];
const u16 guile_duration[259];
const u32 guile_times[259];
const u16 guile_size;
const u8 guile_velocity;
const u16 guile_delay;
const u8 guile_loops;

const Note castle_notes[443];
const u16 castle_duration[443];
const u32 castle_times[443];
const u16 castle_size;
const u8 castle_velocity;
const u16 castle_delay;
const u8 castle_loops;

const Note zelda_notes[491];
const u16 zelda_duration[491];
const u32 zelda_times[491];
const u16 zelda_size;
const u8 zelda_velocity;
const u16 zelda_delay;
const u8 zelda_loops;

const Note topGear_notes[1196];
const u16 topGear_duration[1196];
const u32 topGear_times[1196];
const u16 topGear_size;
const u8 topGear_velocity;
const u16 topGear_delay;
const u8 topGear_loops;

const Note alteredBeast_notes[125];
const u16 alteredBeast_duration[125];
const u32 alteredBeast_times[125];
const u16 alteredBeast_size;
const u8 alteredBeast_velocity;
const u16 alteredBeast_delay;
const u8 alteredBeast_loops;

const Note smooth_notes[404];
const u16 smooth_duration[404];
const u32 smooth_times[404];
const u16 smooth_size;
const u8 smooth_velocity;
const u16 smooth_delay;
const u8 smooth_loops;

const Note phantasy_notes[119];
const u16 phantasy_duration[119];
const u32 phantasy_times[119];
const u16 phantasy_size;
const u8 phantasy_velocity;
const u16 phantasy_delay;
const u8 phantasy_loops;

#endif