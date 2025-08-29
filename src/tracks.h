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

extern const u32 sonic_times[434];
extern const Note sonic_notes[434];
extern const u16 sonic_duration[434];
extern const u16 sonic_size;
extern const u8 sonic_velocity;
extern const u16 sonic_delay;
extern const u8 sonic_loops;


extern const Note bib_notes[223];
extern const u16 bib_duration[223];
extern const u32 bib_times[223];
extern const u16 bib_size;
extern const u8 bib_velocity;
extern const u16 bib_delay;
extern const u8 bib_loops;

extern const Note sor_notes[165];
extern const u16 sor_duration[165];
extern const u32 sor_times[165];
extern const u16 sor_size;
extern const u8 sor_velocity;
extern const u16 sor_delay;
extern const u8 sor_loops;

extern const Note guile_notes[259];
extern const u16 guile_duration[259];
extern const u32 guile_times[259];
extern const u16 guile_size;
extern const u8 guile_velocity;
extern const u16 guile_delay;
extern const u8 guile_loops;

extern const Note castle_notes[443];
extern const u16 castle_duration[443];
extern const u32 castle_times[443];
extern const u16 castle_size;
extern const u8 castle_velocity;
extern const u16 castle_delay;
extern const u8 castle_loops;

extern const Note zelda_notes[491];
extern const u16 zelda_duration[491];
extern const u32 zelda_times[491];
extern const u16 zelda_size;
extern const u8 zelda_velocity;
extern const u16 zelda_delay;
extern const u8 zelda_loops;

extern const Note topGear_notes[1196];
extern const u16 topGear_duration[1196];
extern const u32 topGear_times[1196];
extern const u16 topGear_size;
extern const u8 topGear_velocity;
extern const u16 topGear_delay;
extern const u8 topGear_loops;

extern const Note alteredBeast_notes[125];
extern const u16 alteredBeast_duration[125];
extern const u32 alteredBeast_times[125];
extern const u16 alteredBeast_size;
extern const u8 alteredBeast_velocity;
extern const u16 alteredBeast_delay;
extern const u8 alteredBeast_loops;

extern const Note smooth_notes[404];
extern const u16 smooth_duration[404];
extern const u32 smooth_times[404];
extern const u16 smooth_size;
extern const u8 smooth_velocity;
extern const u16 smooth_delay;
extern const u8 smooth_loops;

extern const Note phantasy_notes[119];
extern const u16 phantasy_duration[119];
extern const u32 phantasy_times[119];
extern const u16 phantasy_size;
extern const u8 phantasy_velocity;
extern const u16 phantasy_delay;
extern const u8 phantasy_loops;

#endif