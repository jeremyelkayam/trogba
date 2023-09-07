#pragma once 
#include <bn_sound_items.h>

namespace trog {

struct voice_clips{
    bn::sound_item gameover, archergameover, highscore, secretscore, arrowed, 
        majesty, majesty_troghammer, ignitecottage, selection, sworded[2], burninate[3], 
        levelbeat[2], ignitepeasant[2], stomp[3];
};

// Lookup table for voice clips
// Organized by dragon, then purpose
const voice_clips all_clips[3] { 
    //trogdor
    {
        bn::sound_items::sb_pineforyou, //game over
        bn::sound_items::sb_archer_gameover, //archer game over
        bn::sound_items::sb_beatthatscore, //high score
        bn::sound_items::sb_secretthing, // secret score
        bn::sound_items::sb_stupid_archdeluxe, //arrowed
        bn::sound_items::sb_ohyeah, //majesty
        bn::sound_items::sb_closeone, //majesty troghammer
        bn::sound_items::sb_dooj, //ignite cottage
        bn::sound_items::sb_strikesagain, // selection
        //sworded
        {bn::sound_items::sb_worstgame, bn::sound_items::sb_stupidknight},
        //burninate
        {bn::sound_items::sb_tablesturn, bn::sound_items::sb_advantage, bn::sound_items::sb_lowres},
        //level beat
        {bn::sound_items::sb_comesinthenight, bn::sound_items::sb_bestgame},
        //ignite peasant
        {bn::sound_items::sb_hesonfire, bn::sound_items::sb_lookit_him_burning},
        //stomp
        {bn::sound_items::sb_asquisha, bn::sound_items::sb_asquishasquisha, bn::sound_items::sb_squishedyou}
    },
    //sucks
    {
        bn::sound_items::sb_itsover, //game over
        bn::sound_items::sb_boo, //archer game over
        bn::sound_items::sb_impressive, //high score
        bn::sound_items::sb_highscore, // secret score
        bn::sound_items::sb_arrowed, //arrowed
        bn::sound_items::sb_thread_needle, //majesty
        bn::sound_items::sb_closeone, //majesty troghammer
        bn::sound_items::sb_burncottage, //ignite cottage
        bn::sound_items::sb_unnatural, // selection
        //sworded
        {bn::sound_items::sb_sordid_affair, bn::sound_items::sb_knight_looking},
        //burninate
        {bn::sound_items::sb_getburning, bn::sound_items::sb_style, bn::sound_items::sb_snakefire},
        //level beat
        {bn::sound_items::sb_sucks_clap, bn::sound_items::sb_sisforsucks},
        //ignite peasant
        {bn::sound_items::sb_burninate, bn::sound_items::sb_fiyah},
        //stomp
        {bn::sound_items::sb_rampage, bn::sound_items::sb_stomp, bn::sound_items::sb_herepeasant}
    },
    //chiaroscuro
    {
        bn::sound_items::dummy, //game over
        bn::sound_items::dummy, //archer game over
        bn::sound_items::dummy, //high score
        bn::sound_items::dummy, // secret score
        bn::sound_items::dummy, //arrowed
        bn::sound_items::dummy, //majesty
        bn::sound_items::dummy, //majesty troghammer
        bn::sound_items::dummy, //ignite cottage
        bn::sound_items::dummy, //selection
        //sworded
        {bn::sound_items::dummy, bn::sound_items::dummy},
        //burninate
        {bn::sound_items::dummy, bn::sound_items::dummy, bn::sound_items::dummy},
        //level beat
        {bn::sound_items::dummy, bn::sound_items::dummy},
        //ignite peasant
        {bn::sound_items::dummy, bn::sound_items::dummy},
        //stomp
        {bn::sound_items::dummy, bn::sound_items::dummy, bn::sound_items::dummy}
    }
};

}