#ifndef __LIST_H__
#define __LIST_H__

#include <genesis.h>
#include <memory.h>
#include "tracks.h"


#define WIDTH 320
#define HEIGHT 224
#define TARGET_HEIGHT 195
#define TRACK_HEIGHT 62

// define the node structure
typedef struct Temp
{
    Sprite *sprite;
    f16 x;
    s16 y;
    Note type;
    struct Temp *next;
} LNotes;

// define the node structure
typedef struct TempB
{
    Sprite *sprite;
    f16 x;
    s16 y;
    Note type;
    s16 duration;
    struct TempB *next;
} LBars;



extern LNotes *note_start;
extern LBars *bar_start;

extern Sprite *fireR, *fireG, *fireY;

void createList_Note();
u8 insert_Note(Sprite *sprite, f16 x, s16 y, Note type);
s16 updatePosition_Note(u8 velocity, s16 score);
u16 listSize_Note(u8 list);

void createList_Bar();
u8 insert_Bar(Sprite *sprite, f16 x, s16 y, Note type, s16 duration);
s16 updatePosition_Bar(u8 velocity, s16 score);

void clear_lists();

void hide_fire();

#endif