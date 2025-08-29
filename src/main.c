/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <string.h>
#include "tracks.h"
#include "bg.h"
#include "bt.h"
#include "control.h"
#include <timer.h>
#include "sound.h"
#include "list.h"
#include "menu.h"

#define SFX_ERROR 64
#define SFX_CLICK 65

#define SCORE_Y 7

#define VU_HEIGHT TARGET_HEIGHT - 100
#define MULTIPLIER_HEIGHT VU_HEIGHT - 18
#define GUITARIST_HEIGHT VU_HEIGHT - 42


Sprite *btr2, *btg2, *bty2;
Sprite *cursor;
Sprite *vu;
Sprite *mult_s;
Sprite *guitarrista;

char text[64];


enum States
{
    CREDITS,
    MAIN_MENU,
    GAME,
    TRACKS,
    PAUSE,
    TRACK_END,
};

int main(_Bool)
{
    SYS_disableInts();
    VDP_init();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    SYS_enableInts();

    XGM_setPCM(SFX_ERROR, error_sfx, sizeof(error_sfx));
    XGM_setPCM(SFX_CLICK, click_sfx, sizeof(click_sfx));

    //inicializacao de sprites
    SPR_init();

    JOY_init();
    control_initializeVariables();
    JOY_setEventHandler(&control_Handler);

    s16 score = 0;
    s8 consecutivas = 0;
    u8 multiplicador = 1;
    bool perdeu = FALSE;

    u32 init_time = getTick();
    u32 pause_time = 0;
    u32 final_time = 0xFFFFFFFF;
    bool start_music = 0;
    bool notes_remaining = 1;
    u16 nota_index = 0;
    bool resume = 0;

    u32 creditos_time = 0xFFFFFFFF;

    enum States state = MAIN_MENU;
    enum States state_anterior = -1;

    Music music = BACK_IN_BLACK;

    const Note *notes;
    const u32 *times;
    const u16 *durations;
    u16 music_size;
    u8 velocity;
    const u8 *music_xgm;
    u16 delay;
    u8 loops;

    s16 cursorY;
    s16 cursorX;
    s16 selected_music_index;

    s16 yOffsetBg = 0;
    
    s16 xOffsetsky[32]= {0};

    s8 menu_movendo = 0;

    const u16 k7_colors[] = {0x02e, 0xa00, 0x06a, 0x0a0};
    u8 color_index;
    
    const s16 offset_mask [28] = {19, 17, 15, 14, 12, 10, 9, 8, 7, 6, 6, 5, 5, 5, 5, 5, 5, 6, 6, 7, 8, 9, 10, 12, 14, 15, 17, 19};

    while (1)
    {
        switch (state)
        {
            case CREDITS:
            if (state_anterior != state)
            {
                VDP_clearPlane(BG_A, TRUE);
                VDP_setVerticalScroll(BG_A, 0);
                
                state_anterior = state;
                VDP_drawImageEx(BG_A, &creditos, TILE_ATTR(PAL0, FALSE, FALSE, FALSE), 0, 0, TRUE, TRUE);
                creditos_time = getTick();
            }
            if(getTick() - creditos_time > 900)
            {
                PAL_fadeOutPalette(PAL0, 60, FALSE);
                state = MAIN_MENU;
            }
            break;
        case MAIN_MENU:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_B);
                // VDP_clearPlane(BG_A, TRUE);
                // VDP_setVerticalScroll(BG_A, 0);
                VDP_resetScreen();
                PAL_setColors(0, (u16 *)palette_black, 64, DMA); // set all palettes to black
                
                PAL_setPalette(PAL3, Cursor.palette->data, DMA);

                show_initial_menu();
                cursor = SPR_addSprite(&Cursor, 10 * 8, 14 * 8, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                state_anterior = state;
                VDP_drawImageEx(BG_A, &concert, TILE_ATTR(PAL0, FALSE, FALSE, FALSE), 0, 0, FALSE, TRUE);
                PAL_fadeInPalette(PAL0, concert.palette->data, 20, FALSE);
                PAL_setPalette(PAL0, concert.palette->data, DMA);
                PAL_setColor(15, RGB24_TO_VDPCOLOR(0xffFFFF));
            }
            if (J1S)
            {
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
                J1S = 0;
                state = TRACKS;
                SPR_releaseSprite(cursor);
                VDP_clearTextLine(14);
            }          
            break;
        case TRACKS:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_B);
                yOffsetBg =0;
                VDP_setScrollingMode(HSCROLL_TILE , VSCROLL_PLANE);
                VDP_clearPlane(BG_A, TRUE);
                VDP_clearPlane(BG_B, TRUE);
                VDP_setVerticalScroll(BG_A, yOffsetBg);
                PAL_setColors(0, (u16 *)palette_black, 64, DMA); // set all palettes to black
                PAL_setColor(15, 0x00e); // set color text red
                
                cursorY = 0;
                state_anterior = state;
                J1DOWN = 0;
                J1UP = 0;
                VDP_drawImageEx(BG_A, &k7, TILE_ATTR(PAL1, FALSE, FALSE, FALSE), 15, -9, FALSE, TRUE);
                VDP_drawImageEx(BG_A, &k7, TILE_ATTR(PAL2, FALSE, FALSE, FALSE), 15, 7, FALSE, TRUE);
                VDP_drawImageEx(BG_B, &sonic_cover, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, k7.tileset->numTile), 1, 7, FALSE, TRUE);

                PAL_setPalette(PAL1, k7.palette->data, DMA);
                PAL_setPalette(PAL2, k7.palette->data, DMA);
                PAL_setPalette(PAL3, sonic_cover.palette->data, DMA);
                VDP_drawText(music_options[cursorY].text, music_options[cursorY].x,music_options[cursorY ].y);
                VDP_drawText(music_options[cursorY + 1].text, music_options[cursorY + 1].x, 26);
                menu_movendo = 0;
                color_index = 0;
                VDP_setHorizontalScrollTile(BG_A, 0, offset_mask, 28, DMA);
            }
            if (J1DOWN)
            {
                menu_movendo = 1;
                J1DOWN = 0;
                if (cursorY < TRACK_COUNT -1)
                {
                    cursorY++;
                }
                else
                {
                    cursorY = 0;
                    
                }
                if( color_index < 3)
                {
                    color_index++;
                }
                else
                {
                    color_index =0;
                }
           
                VDP_clearTextArea(music_options[cursorY].x-2, music_options[cursorY].y, 32, 1);
                VDP_clearTextArea(music_options[cursorY].x-2, 26, 32, 1);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
                PAL_setColor(19, k7_colors[color_index]);
                VDP_clearPlane(BG_B, TRUE);
                VDP_drawImageEx(BG_B, music_options[cursorY].image, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, k7.tileset->numTile), 1, 7, FALSE, TRUE);
                PAL_setPalette(PAL3, music_options[cursorY].image->palette->data, DMA);      
            }
            if (J1UP)
            {
                menu_movendo = -1;
                J1UP = 0;
                if (cursorY > 0)
                {
                    cursorY--;
                }
                else
                {
                    cursorY = TRACK_COUNT -1;
                }
                if( color_index > 0)
                {
                    color_index--;
                }
                else
                {
                    color_index = 3;
                }

                VDP_clearTextArea(music_options[cursorY].x-2, music_options[cursorY].y, 32, 1);
                VDP_clearTextArea(music_options[cursorY].x-2, 26, 32, 1);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
                PAL_setColor(19, k7_colors[color_index]);  
                VDP_clearPlane(BG_B, TRUE);
                VDP_drawImageEx(BG_B, music_options[cursorY].image, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, k7.tileset->numTile), 1, 7, FALSE, TRUE);
                PAL_setPalette(PAL3, music_options[cursorY].image->palette->data, DMA);   
            }
            if(menu_movendo)
            {
                yOffsetBg = yOffsetBg + 8*menu_movendo;
                VDP_setVerticalScroll(BG_A, yOffsetBg);
                // VDP_setHorizontalScrollTile(BG_A, 0, offset_mask, 28, DMA);
                if (yOffsetBg == 128 || yOffsetBg == -128)
                {
                    yOffsetBg = 0;
                    menu_movendo =0;
                    VDP_drawText(music_options[cursorY].text, music_options[cursorY].x, music_options[cursorY].y);
                    if (cursorY == TRACK_COUNT - 1)
                    {
                        VDP_drawText(music_options[0].text, music_options[0].x + 1, 26);
                    }
                    else
                    {
                        VDP_drawText(music_options[cursorY +1].text, music_options[cursorY +1].x + 1,26);
                    }
                    PAL_setColor(35, k7_colors[color_index]);
                }
            }

            if (J1S | J1A | J1B | J1C)
            {
                J1S = 0;
                J1A = 0;
                J1B = 0;
                J1C = 0;

                // SPR_releaseSprite(cursor);
                VDP_clearTextLine(music_options[cursorY].y);
                VDP_clearTextArea(music_options[cursorY].x -2, 26, 32, 1);
                VDP_setHorizontalScroll(BG_A, 0); //reset horizontal scroll
                
                // Save the selected music index
                selected_music_index = cursorY;
                
                //selec sonic
                if (cursorY == 0)
                {
                    music = SONIC;
                }
                //select back in black
                else if (cursorY == 1)
                {
                    music = BACK_IN_BLACK;
                }
                else if (cursorY == 2)
                {
                    music = STREETS_OF_RAGE;
                }
                else if (cursorY == 3)
                {
                    music = GUILE_THEME;
                }
                else if (cursorY == 4)
                {
                    music = ZELDA;
                }
                else if (cursorY == 5)
                {
                    music = DRACULA;
                }
                else if (cursorY == 6)
                {
                    music = TOP_GEAR;
                }
                else if (cursorY == 7)
                {
                    music = ALTERED;
                }
                else if (cursorY == 8)
                {
                    music = SMOOTH;
                }
                else if (cursorY == 9)
                {
                    music = PHATASY;
                }
                state = GAME;
            }
            break;
        case GAME:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_A);
                if (resume == 0)
                {
                    VDP_resetScreen();
                    // VDP_setPlaneSize(32, 64, TRUE);
                    VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);

                    VDP_drawImageEx(BG_A, &blank, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, 0), 0, 0, FALSE, TRUE);
                    VDP_drawImageEx(BG_B, &bg_musica, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, blank.tileset->numTile), 0, 0, FALSE, TRUE);
                    
                    PAL_setPalette(PAL3, Mult.palette->data, DMA);
                    PAL_setPalette(PAL1, bg_musica.palette->data, DMA);
                    PAL_setPalette(PAL2, btR2.palette->data, DMA);
                    PAL_setPalette(PAL0, Fogo.palette->data, DMA);
                    PAL_setColor(15, RGB24_TO_VDPCOLOR(0xff0000));
                    PAL_setColor(0, 0);
                    VDP_setBackgroundColor(0);
                    // bts_marca
                    btr2 = SPR_addSprite(&btR2, RED_X+1, TARGET_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                    btg2 = SPR_addSprite(&btG2, GREEN_X+1, TARGET_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                    bty2 = SPR_addSprite(&btY2, YELLOW_X+2, TARGET_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


                    fireR = SPR_addSprite(&Fogo, RED_X+7, TARGET_HEIGHT-26, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
                    fireG = SPR_addSprite(&Fogo, GREEN_X+7, TARGET_HEIGHT-26, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
                    fireY = SPR_addSprite(&Fogo, YELLOW_X+8, TARGET_HEIGHT-26, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));

                    vu = SPR_addSprite(&Vu, 270 , VU_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                    mult_s = SPR_addSprite(&Mult, 290 , MULTIPLIER_HEIGHT, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                    guitarrista = SPR_addSprite(&Guitar,  260 , GUITARIST_HEIGHT, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));

                    SPR_setVisibility(fireR, HIDDEN);
                    SPR_setVisibility(fireG, HIDDEN);
                    SPR_setVisibility(fireY, HIDDEN);

                    SPR_setDepth(btr2, SPR_MAX_DEPTH);
                    SPR_setDepth(btg2, SPR_MAX_DEPTH);
                    SPR_setDepth(bty2, SPR_MAX_DEPTH);

                    SPR_setDepth(fireR, SPR_MIN_DEPTH);
                    SPR_setDepth(fireG, SPR_MIN_DEPTH);
                    SPR_setDepth(fireY, SPR_MIN_DEPTH);

                    nota_index = 0;
                    score = 0;
                    consecutivas = 0;
                    multiplicador = 1;

                    init_time = getTick();
                    start_music = 0;
                    pause_time = 0;
                    notes_remaining = 1;

                    switch (music)
                    {
                    case SONIC:
                        notes = sonic_notes;
                        times = sonic_times;
                        music_size = sonic_size;
                        velocity = sonic_velocity;
                        durations = sonic_duration;
                        music_xgm = sonic_music;
                        delay = sonic_delay;
                        loops = sonic_loops;
                        break;

                    case BACK_IN_BLACK:
                        notes = bib_notes;
                        times = bib_times;
                        music_size = bib_size;
                        velocity = bib_velocity;
                        durations = bib_duration;
                        music_xgm = back_music;
                        delay = bib_delay;
                        loops = bib_loops;
                        break;

                    case STREETS_OF_RAGE:
                        notes = sor_notes;
                        times = sor_times;
                        music_size = sor_size;
                        velocity = sor_velocity;
                        durations = sor_duration;
                        music_xgm = sor_music;
                        delay = sor_delay;
                        loops = sor_loops;
                        break;
                    
                    case GUILE_THEME:
                        notes = guile_notes;
                        times = guile_times;
                        music_size = guile_size;
                        velocity = guile_velocity;
                        durations = guile_duration;
                        music_xgm = guile_music;
                        delay = guile_delay;
                        loops = guile_loops;
                        break;
                    
                    case ZELDA:
                        notes = zelda_notes;
                        times = zelda_times;
                        music_size = zelda_size;
                        velocity = zelda_velocity;
                        durations = zelda_duration;
                        music_xgm = zelda_music;
                        delay = zelda_delay;
                        loops = zelda_loops;
                        break;
                    
                    case DRACULA:
                        notes = castle_notes;
                        times = castle_times;
                        music_size = castle_size;
                        velocity = castle_velocity;
                        durations = castle_duration;
                        music_xgm = castle_music;
                        delay = castle_delay;
                        loops = castle_loops;
                        break;
                    
                    case TOP_GEAR:
                        notes = topGear_notes;
                        times = topGear_times;
                        music_size = topGear_size;
                        velocity = topGear_velocity;
                        durations = topGear_duration;
                        music_xgm = topGear_music;
                        delay = topGear_delay;
                        loops = topGear_loops;
                        break;
                    
                    case ALTERED:
                        notes = alteredBeast_notes;
                        times = alteredBeast_times;
                        music_size = alteredBeast_size;
                        velocity = alteredBeast_velocity;
                        durations = alteredBeast_duration;
                        music_xgm = ateredBeast_music;
                        delay = alteredBeast_delay;
                        loops = alteredBeast_loops;
                        break;
                    
                    case SMOOTH:
                        notes = smooth_notes;
                        times = smooth_times;
                        music_size = smooth_size;
                        velocity = smooth_velocity;
                        durations = smooth_duration;
                        music_xgm = smooth_music;
                        delay = smooth_delay;
                        loops = smooth_loops;
                        break;
                    
                     case PHATASY:
                        notes = phantasy_notes;
                        times = phantasy_times;
                        music_size = phantasy_size;
                        velocity = phantasy_velocity;
                        durations = phantasy_duration;
                        music_xgm = phantasy_music;
                        delay = phantasy_delay;
                        loops = phantasy_loops;
                        break;

                    default:
                        break;
                    }

                    createList_Note();
                    createList_Bar();
                }
                else
                {
                    resume = 0;
                }

                state_anterior = state;
            }
            if (getTick() - init_time >= delay && !start_music)
            {
                XGM_startPlay(music_xgm);
                XGM_setLoopNumber(loops);

                start_music = 1;
            }
            while (notes_remaining && (getTick() - init_time >= times[nota_index]))
            {
                if (notes[nota_index] & YELLOW)
                {
                    insert_Note(SPR_addSprite(&btY, 0, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), YELLOW_X_START, TRACK_HEIGHT, YELLOW);
                    if (durations[nota_index] > 0)
                    {
                        insert_Bar(SPR_addSprite(&barraY, YELLOW_B_X, TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), YELLOW_B_X_START, TRACK_HEIGHT, YELLOW, durations[nota_index]);
                    }
                }
                else if (notes[nota_index] & GREEN)
                {
                    insert_Note(SPR_addSprite(&btG, GREEN_X, TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), GREEN_X_START, TRACK_HEIGHT, GREEN);
                    if (durations[nota_index] > 0)
                    {
                        insert_Bar(SPR_addSprite(&barraG, GREEN_B_X, TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), GREEN_B_X_START, TRACK_HEIGHT, GREEN, durations[nota_index]);
                    }
                }
                else if (notes[nota_index] & RED)
                {
                    insert_Note(SPR_addSprite(&btR, RED_X, TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), RED_X_START, TRACK_HEIGHT, RED);
                    if (durations[nota_index] > 0)
                    {
                        insert_Bar(SPR_addSprite(&barraR, RED_B_X, TRACK_HEIGHT, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), RED_B_X_START, TRACK_HEIGHT, RED, durations[nota_index]);
                    }
                }
                if (nota_index < music_size-1)
                {
                    nota_index++;
                }
                else
                {
                    notes_remaining = 0;
                    final_time = getTick();
                    break;
                }
            }
            s8 partial = 0;
            partial = updatePosition_Note(velocity, partial);
            if (partial > 0)
            {
                score = score + multiplicador;
                if (multiplicador < 4)
                {
                    if(vu->frameInd == 10)
                    {
                        SPR_nextFrame(vu);
                    }
                    consecutivas++;
                    SPR_nextFrame(vu);

                    if (consecutivas == 10)
                    {
                        multiplicador++;
                        SPR_nextFrame(mult_s);
                        consecutivas = 0;
                    }

                }
            }
            else if (partial < 0)
            {
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                KLog_S2("consecutivas: ",consecutivas," multiplicador: ", multiplicador);
                if(consecutivas < 1 && multiplicador == 1)
                {
                    consecutivas --;
                }
                else
                {
                    consecutivas = 0;
                    multiplicador = 1;
                }
                if(consecutivas == -5)
                {
                    perdeu = TRUE;
                }
            }

            hide_fire();

            partial = 0;
            partial = updatePosition_Bar(velocity, partial);
            score = partial * multiplicador + score;

            if (J1A && (J1ACount + 50) > (u16)getTick())
            {
                score--;
                J1A = 0;
                consecutivas = 0;
                multiplicador = 1;
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
            }
            if (J1B && (J1BCount + 50) > (u16)getTick())
            {
                score--;
                J1B = 0;
                consecutivas = 0;
                multiplicador = 1;
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
            }
            if (J1C && (J1CCount + 50) > (u16)getTick())
            {
                score--;
                J1C = 0;
                consecutivas = 0;
                multiplicador = 1;
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
            }

            SPR_setAnim(guitarrista, multiplicador - 1);

            if (J1S)
            {
                J1S = 0;
                state = PAUSE;
                pause_time = getTick();
            }

            u16 JOY1 = JOY_readJoypad(JOY_1);
            if ((JOY1 & BUTTON_A))
            {
                SPR_setFrame(bty2, 1);
            }
            else
            {
                SPR_setFrame(bty2, 0);
            }
            if ((JOY1 & BUTTON_B))
            {
                SPR_setFrame(btg2, 1);
            }
            else
            {
                SPR_setFrame(btg2, 0);
            }
            if ((JOY1 & BUTTON_C))
            {
                SPR_setFrame(btr2, 1);
            }
            else
            {
                SPR_setFrame(btr2, 0);
            }

            yOffsetBg -= 1;
            if (yOffsetBg < 0)
            {
                yOffsetBg += 5120;
            }
            // VDP_setVerticalScroll(BG_A, yOffsetBg);
            memsetU16(xOffsetsky, yOffsetBg/10, 9);
            // VDP_setHorizontalScrollTile(BG_A, 0, xOffsetsky, 9, DMA);

            // draw screen
            sprintf(text, "%05d", score);
            VDP_drawText(text, 34, SCORE_Y);

            if((notes_remaining  == 0 && getTick() - final_time > 1000) || perdeu)
            {
                state = TRACK_END;
                perdeu = FALSE;

                clear_lists();
                SPR_releaseSprite(btr2);
                SPR_releaseSprite(btg2);
                SPR_releaseSprite(bty2);

                SPR_releaseSprite(fireR);
                SPR_releaseSprite(fireG);
                SPR_releaseSprite(fireY);

                SPR_releaseSprite(vu);
                SPR_releaseSprite(mult_s);
                SPR_releaseSprite(guitarrista);
                VDP_clearTextLine(SCORE_Y);

                XGM_stopPlay();

                SPR_reset();
                MEM_pack();
            }

            break;
        case PAUSE:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_A);
                state_anterior = state;
                show_pause_menu();
                // PAL_setPalette(PAL3, Cursor.palette->data, DMA);
                cursorY = 14;
                cursorX = 17 * 8;
                cursor = SPR_addSprite(&Cursor, cursorX, cursorY * 8, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                XGM_pausePlay();
                guitarrista->timer = 0;
                fireR->timer = 0;
                fireG->timer = 0;
                fireY->timer = 0;
            }
            if (J1DOWN)
            {
                J1DOWN = 0;
                if (cursorY < 16)
                {
                    cursorY++;
                }
                else
                {
                    cursorY = 14;
                }
                SPR_setPosition(cursor, cursorX, cursorY * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1UP)
            {
                J1UP = 0;
                if (cursorY > 14)
                {
                    cursorY--;
                }
                else
                {
                    cursorY = 16;
                }
                SPR_setPosition(cursor, cursorX, cursorY * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1S | J1A | J1B | J1C)
            {
                J1S = 0;
                J1A = 0;
                J1B = 0;
                J1C = 0;

                SPR_releaseSprite(cursor);
                VDP_clearTextLine(14);
                VDP_clearTextLine(15);
                VDP_clearTextLine(16);

                //voltar
                if (cursorY == 14)
                {
                    state = GAME;
                    resume = 1;
                    init_time = getTick() - pause_time + init_time;
                    XGM_resumePlay();
                    guitarrista->timer = 1;
                    fireR->timer = 1;
                    fireG->timer = 1;
                    fireY->timer = 1;
                }
                //sair
                else if (cursorY == 15)
                {
                    state = MAIN_MENU;
                    clear_lists();
                    SPR_releaseSprite(btr2);
                    SPR_releaseSprite(btg2);
                    SPR_releaseSprite(bty2);

                    SPR_releaseSprite(fireR);
                    SPR_releaseSprite(fireG);
                    SPR_releaseSprite(fireY);

                    SPR_releaseSprite(vu);
                    SPR_releaseSprite(mult_s);
                    SPR_releaseSprite(guitarrista);
                    VDP_clearTextLine(SCORE_Y);

                    XGM_stopPlay();

                    SPR_reset();
                    MEM_pack();
                }
                //reiniciar
                else
                {
                    state = GAME;
                    clear_lists();
                    SPR_releaseSprite(btr2);
                    SPR_releaseSprite(btg2);
                    SPR_releaseSprite(bty2);

                    SPR_releaseSprite(fireR);
                    SPR_releaseSprite(fireG);
                    SPR_releaseSprite(fireY);

                    SPR_releaseSprite(vu);
                    SPR_releaseSprite(mult_s);
                    SPR_releaseSprite(guitarrista);
                    
                    XGM_stopPlay();

                    SPR_reset();
                    MEM_pack();
                }
            }
            break;
        case TRACK_END:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_B);
                VDP_resetScreen();
                PAL_setColors(0, (u16 *)palette_black, 64, DMA); // set all palettes to black
                state_anterior = state;
                
                // Show current track name
                VDP_drawText(music_options[selected_music_index].text, music_options[selected_music_index].x, music_options[selected_music_index].y);
                
                // Show end menu options
                show_menu(end_options, NUM_END_OPTIONS);
                PAL_setPalette(PAL3, Cursor.palette->data, DMA);
                cursorY = 0; // Reset cursor to first end option (EXIT)
                cursorX = 17 * 8;
                cursor = SPR_addSprite(&Cursor, cursorX, 14 * 8, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                XGM_stopPlay();

                VDP_drawImageEx(BG_A, &concert, TILE_ATTR(PAL0, FALSE, FALSE, FALSE), 0, 0, FALSE, TRUE);
                PAL_setPalette(PAL0, concert.palette->data,DMA);

                sprintf(text, "Your score was: %05d", score);
                VDP_drawText(text, 10, 11);
                PAL_setColor(15, RGB24_TO_VDPCOLOR(0xffFFFF));
            }
            if (J1DOWN)
            {
                J1DOWN = 0;
                if (cursorY < NUM_END_OPTIONS - 1)
                {
                    cursorY++;
                }
                else
                {
                    cursorY = 0;
                }
                SPR_setPosition(cursor, cursorX, (14 + cursorY) * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1UP)
            {
                J1UP = 0;
                if (cursorY > 0)
                {
                    cursorY--;
                }
                else
                {
                    cursorY = NUM_END_OPTIONS - 1;
                }
                SPR_setPosition(cursor, cursorX, (14 + cursorY) * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1S | J1A | J1B | J1C)
            {
                J1S = 0;
                J1A = 0;
                J1B = 0;
                J1C = 0;

                SPR_releaseSprite(cursor);
                VDP_clearTextLine(11);
                VDP_clearTextLine(14);
                VDP_clearTextLine(15);

                 if (cursorY == 1) // RESTART option
                 {
                    state = GAME;
                 }
                 else // EXIT option (cursorY == 0)
                 {
                    state = MAIN_MENU;
                 }
            }
            break;
        }
        SPR_update();
        // KLog_S1("sprites: \0", SPR_getNumActiveSprite());
        SYS_doVBlankProcess();
        // VDP_showFPS()
        // VDP_showCPULoad();
    }
    return (0);
}
