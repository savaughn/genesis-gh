#include "list.h"
#include "control.h"
#include "timer.h"
#include "tracks.h"
#include "bt.h"

#define FIRE_TIME 110
#define DIFY FIX16(TARGET_HEIGHT) - FIX16(TRACK_HEIGHT)
#define DIFX YELLOW_X_START - FIX16(YELLOW_X)
#define SCALE F16_div(DIFX, DIFY)

// Define the actual variables
LNotes *note_start;
LBars *bar_start;
Sprite *fireR, *fireG, *fireY;

static u32 fireTimeR = 0, fireTimeG = 0, fireTimeY = 0;

//--------------------------------------------------------
// Function that defines the list as empty.
void createList_Note()
{
    note_start = NULL;
}
//--------------------------------------------------------
// Function that inserts an element at the beginning of the list.
// Returns:
//	0 - if there is no memory to insert
//	1 - if managed to insert
u8 insert_Note(Sprite *sprite, f16 x, s16 y, Note type)
{
    LNotes *p;
    // MEM_pack();
    p = (LNotes *)MEM_alloc(sizeof(LNotes));
    if (p == NULL || sprite == NULL)
    {
        // allocation error
        // KLog("error in allocation");
        return 0;
    }
    
    p->sprite = sprite;
    p->x = x;
    p->y = y;
    p->type =  type;
    p->next = NULL;

    SPR_setDepth(sprite, SPR_MIN_DEPTH);

    if (note_start == NULL)
        note_start = p;
    else
    {
        p->next = note_start;
        note_start = p;
    }
    return 1;
}



//--------------------------------------------------------
// Function that defines the list as empty.
void createList_Bar()
{
    bar_start = NULL;
}
//--------------------------------------------------------
// Function that inserts an element at the beginning of the list.
// Returns:
//	0 - if there is no memory to insert
//	1 - if managed to insert
u8 insert_Bar(Sprite *sprite, f16 x, s16 y, Note type, s16 duration)
{
    LBars *p;
    
    // MEM_pack();
    p = (LBars *)MEM_alloc(sizeof(LBars));
    if (p == NULL || sprite == NULL)
    {
        // allocation error
        // KLog("error in allocation");
        return 0;
    }
    
    p->sprite = sprite;
    p->x = x;
    p->y = y;
    p->type = type;
    p->duration = duration;
    p->next = NULL;

    SPR_setDepth(sprite, SPR_MIN_DEPTH);

    if (bar_start == NULL)
        bar_start = p;
    else
    {
        LBars *current = bar_start;
        while ( current->next != NULL)
        {
            current = current->next;
        }
        current->next = p;
    }
    return 1;
}

//--------------------------------------------------------
// Function tells the size of the list.
// Returns:
//	list size
u16 listSize_Note(u8 list)
{
    u16 size = 0;
    if ( list == 0)
    {
        if (note_start == NULL)
            return 0;
        LNotes *ptr = note_start;
        for (size = 0; ptr != NULL; size++)
        {
            ptr = ptr->next;
        }
    }
    else if(list == 1)
    {
        if (bar_start == NULL)
            return 0;
        LBars *ptr = bar_start;
        for (size = 0; ptr != NULL; size++)
        {
            ptr = ptr->next;
        }
    }
    
    return size;

}


//--------------------------------------------------------
// Function that updates the position of notes and detects if the user pressed the button at the right time.
// Returns:
//number of correct notes, or -1 if a note was missed;
s16 updatePosition_Note(u8 velocity, s16 score)
{
    LNotes *ptr, *before;
    if (note_start == NULL)
    {
        return score; // Empty list !!!
    }
    else
    {
        ptr = note_start;
        before = note_start;

        f16 v = FIX16(velocity);
        bool update = FALSE;
        while (ptr != NULL)
        {
            ptr->y = ptr->y + velocity;
            if (ptr->y >= HEIGHT) // passed the limit
            {
                if(SPR_isVisible(ptr->sprite, 1))
                {
                    score --;
                }
                if (ptr == note_start) // if removing the first in the list
                {
                    note_start = note_start->next;
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr);
                    ptr = note_start;
                    update = TRUE;
                }
                else // removing from the middle of the list
                {
                    before->next = ptr->next; // Redo the link
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr); // Free the node area
                    ptr = before->next;
                    update = TRUE;
                }
            }
            else
            {
                if(ptr->type == YELLOW)
                {
                    ptr->x = ptr->x - F16_mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  F16_toInt(ptr->x), ptr->y);
                }
                else if(ptr->type == RED)
                {
                    ptr->x = ptr->x + F16_mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  F16_toInt(ptr->x), ptr->y);
                }
                else
                {
                    SPR_setPosition(ptr->sprite,  F16_toInt(ptr->x), ptr->y);
                } 
                if((ptr->y - TRACK_HEIGHT) % 19 == 0 && ptr->y < 214) // Divide the track into 7 segments of 19px height each with a note size
                {
                    SPR_setFrame(ptr->sprite, (ptr->y - TRACK_HEIGHT)/19);
                }
                if(ptr->y > TARGET_HEIGHT - 15 && ptr->y < TARGET_HEIGHT + 15 )
                {
                    if(SPR_isVisible(ptr->sprite, 1))
                    {
                        // increase score and make sprite invisible 
                        if( ptr->type == YELLOW && J1A )
                        {
                            score++;
                            J1A = 0;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                            SPR_setFrame(fireY,0);
                            SPR_setVisibility(fireY, VISIBLE);
                            fireTimeY = getTick();
                        }
                        if(ptr->type == GREEN && J1B)
                        {
                            score++;
                            J1B = 0;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                            SPR_setFrame(fireG,0);
                            SPR_setVisibility(fireG, VISIBLE);
                            fireTimeG = getTick();
                        }
                        if (ptr->type == RED && J1C )
                        {
                            score++;
                            J1C = 0;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                            SPR_setFrame(fireR,0);
                            SPR_setVisibility(fireR, VISIBLE);
                            fireTimeR = getTick();
                        }
                    }
                }
            }
            if(update == FALSE)
            {    
                before = ptr;
                ptr = ptr->next;
            }
            else
            {
                update = FALSE;
            }
        }
    }
    return score;
}

//--------------------------------------------------------
// Function that updates the position of bars and detects if the user pressed the button at the right time.
// Returns:
//score related to bars;
s16 updatePosition_Bar(u8 velocity, s16 score)
{
    LBars *ptr, *before;
    if (bar_start == NULL)
    {
        return score; // Empty list !!!
    }
    else
    {
        ptr = bar_start;
        before = bar_start;
        f16 v = FIX16(velocity);
        bool update = FALSE;
        while (ptr != NULL)
        {
            ptr->y = ptr->y + velocity;

            if (ptr->y >= HEIGHT) // passed the limit
            {
                if (ptr == bar_start) // if removing the first in the list
                {
                    bar_start = bar_start->next;
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr);
                    ptr = bar_start;
                    update = TRUE;
                }
                else // removing from the middle of the list
                {
                    before->next = ptr->next; // Redo the link
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr); // Free the node area
                    ptr = before->next;
                    update = TRUE;
                }
            }
            else
            {
                if(ptr->type == YELLOW)
                {
                    ptr->x = ptr->x - F16_mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  F16_toInt(ptr->x), ptr->y);
                }
                else if(ptr->type == RED)
                {
                    ptr->x = ptr->x + F16_mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  F16_toInt(ptr->x), ptr->y);
                }
                else
                {
                    SPR_setPosition(ptr->sprite,  F16_toInt(ptr->x), ptr->y);
                }
                if( velocity == 3)
                {
                    if(ptr->duration > 0 && ptr->y == 6 + TRACK_HEIGHT)
                    {
                        if(ptr->type == YELLOW)
                        {
                            insert_Bar(SPR_addSprite(&barraY , F16_toInt(ptr->x), TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), YELLOW_B_X_START, TRACK_HEIGHT, YELLOW, ptr->duration-45);
                        }
                        if(ptr->type == GREEN)
                        {
                            insert_Bar(SPR_addSprite(&barraG , F16_toInt(ptr->x), TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), ptr->x, TRACK_HEIGHT, GREEN, ptr->duration-45);
                        }
                        if(ptr->type == RED)
                        {
                            insert_Bar(SPR_addSprite(&barraR , F16_toInt(ptr->x), TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), RED_B_X_START, TRACK_HEIGHT, RED, ptr->duration-45);
                        }
                    }
                }
                else{
                    if(ptr->duration > 0 && ptr->y == 8 + TRACK_HEIGHT)
                    {
                        if(ptr->type == YELLOW)
                        {
                            insert_Bar(SPR_addSprite(&barraY , F16_toInt(ptr->x), TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), YELLOW_B_X_START, TRACK_HEIGHT, YELLOW, ptr->duration-45);
                        }
                        if(ptr->type == GREEN)
                        {
                            insert_Bar(SPR_addSprite(&barraG , F16_toInt(ptr->x), TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), ptr->x, TRACK_HEIGHT, GREEN, ptr->duration-45);
                        }
                        if(ptr->type == RED)
                        {
                            insert_Bar(SPR_addSprite(&barraR , F16_toInt(ptr->x), TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), RED_B_X_START, TRACK_HEIGHT, RED, ptr->duration-45);
                        }
                    }
                }             
                if(ptr->y > TARGET_HEIGHT - 5 && ptr->y < TARGET_HEIGHT + 7 )
                {
                    u16 JOY1 = JOY_readJoypad(JOY_1);
                    if(SPR_isVisible(ptr->sprite, 1))
                    {
                        // increase score and make sprite invisible                         200
                        if( ptr->type == YELLOW)
                        {
                            if ((JOY1 & BUTTON_A))
                            {
                                score++;
                                SPR_setVisibility(ptr->sprite, HIDDEN);
                                SPR_setFrame(fireY,0);
                                SPR_setVisibility(fireY, VISIBLE);
                                fireTimeY = getTick();
                            }
                            
                        }
                        if(ptr->type == GREEN )
                        {
                            if ((JOY1 & BUTTON_B))
                            {
                                score++;
                                SPR_setVisibility(ptr->sprite, HIDDEN);
                                SPR_setFrame(fireG,0);
                                SPR_setVisibility(fireG, VISIBLE);
                                fireTimeG = getTick();
                            }
                        }
                        if (ptr->type == RED)
                        {
                            if ((JOY1 & BUTTON_C))
                            {
                                score++;
                                SPR_setVisibility(ptr->sprite, HIDDEN);
                                SPR_setFrame(fireR,0);
                                SPR_setVisibility(fireR, VISIBLE);    
                                fireTimeR = getTick();
                            }
                        }
                    }
                }
            }
            if(update == FALSE)
            {    
                before = ptr;
                ptr = ptr->next;
            }
            else
            {
                update = FALSE;
            }
        }
    }
    return score;
}


void clear_lists()
{
    LNotes *ptr;
    LBars *ptr2;
    for (ptr = note_start; ptr != NULL; ptr = note_start)
    {
        note_start = note_start->next;
        SPR_releaseSprite(ptr->sprite);
        MEM_free(ptr);
    }
    
    for (ptr2 = bar_start; ptr2 != NULL; ptr2 = bar_start)
    {
        bar_start = bar_start->next;
        SPR_releaseSprite(ptr2->sprite);
        MEM_free(ptr2);
    }
}

void hide_fire()
{
    u32 time =  getTick();
    if (time - fireTimeR >= FIRE_TIME)
    {
        SPR_setVisibility(fireR, HIDDEN);
    }
    if (time - fireTimeG >= FIRE_TIME)
    {
        SPR_setVisibility(fireG, HIDDEN);
    }
    if (time - fireTimeY >= FIRE_TIME)
    {
        SPR_setVisibility(fireY, HIDDEN);
    }
}