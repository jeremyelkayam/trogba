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
const voice_clips all_clips[4] { 
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
        {bn::sound_items::sb_worstgame, 
            bn::sound_items::sb_stupidknight},
        //burninate
        {bn::sound_items::sb_tablesturn, 
            bn::sound_items::sb_advantage, 
            bn::sound_items::sb_lowres},
        //level beat
        {bn::sound_items::sb_comesinthenight, 
            bn::sound_items::sb_bestgame},
        //ignite peasant
        {bn::sound_items::sb_hesonfire, 
            bn::sound_items::sb_lookit_him_burning},
        //stomp
        {bn::sound_items::sb_asquisha, 
            bn::sound_items::sb_asquishasquisha, 
            bn::sound_items::sb_squishedyou}
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
        {bn::sound_items::sb_sordid_affair, 
            bn::sound_items::sb_knight_looking},
        //burninate
        {bn::sound_items::sb_getburning, 
            bn::sound_items::sb_style, 
            bn::sound_items::sb_snakefire},
        //level beat
        {bn::sound_items::sb_sucks_clap, 
            bn::sound_items::sb_sisforsucks},
        //ignite peasant
        {bn::sound_items::sb_burninate, 
            bn::sound_items::sb_fiyah},
        //stomp
        {bn::sound_items::sb_rampage, 
            bn::sound_items::sb_stomp, 
            bn::sound_items::sb_herepeasant}
    },
    //chiaroscuro
    {
        bn::sound_items::ss_animal, //game over
        bn::sound_items::ss_give_up, //archer game over
        bn::sound_items::ss_happiest, //high score
        bn::sound_items::ss_finest_hour, // secret score
        bn::sound_items::ss_foodanymore, //arrowed
        bn::sound_items::ss_sadflying, //majesty
        bn::sound_items::dummy, //majesty troghammer
        bn::sound_items::ss_ohcottage, //ignite cottage
        bn::sound_items::ss_chiaroscuro, //selection
        //sworded
        {bn::sound_items::ss_aw, 
            bn::sound_items::ss_not_improvement},
        //burninate
        {bn::sound_items::ss_monster, 
            bn::sound_items::ss_eatyou, 
            bn::sound_items::ss_burn_whatever},
        //level beat
        {bn::sound_items::ss_works_every_time, 
            bn::sound_items::ss_great},
        //ignite peasant
        {bn::sound_items::ss_peasantburnination, 
            bn::sound_items::ss_hesburninating},
        //stomp
        {bn::sound_items::ss_betterplague, 
            bn::sound_items::ss_thing_away, 
            bn::sound_items::dummy}
    },
    //wormdingler
    {
        bn::sound_items::cz_bride, //game over
        bn::sound_items::cz_insulted_minutes, //archer game over
        bn::sound_items::cz_most_fun, //high score
        bn::sound_items::cz_what_i_doing, // secret score
        bn::sound_items::cz_rack_battom, //arrowed
        bn::sound_items::cz_flightrisk, //majesty
        bn::sound_items::cz_regret_everything, //majesty troghammer
        bn::sound_items::cz_dazzlemaze, //ignite cottage
        bn::sound_items::cz_shout_wormdingler, //selection
        //sworded
        {bn::sound_items::cz_mean, 
            bn::sound_items::cz_crinkles},
        //burninate
        {bn::sound_items::cz_pyrotechnical, 
            bn::sound_items::cz_whoa_mama, 
            bn::sound_items::cz_chorge},
        //level beat
        {bn::sound_items::cz_goodtime, 
            bn::sound_items::cz_yay},
        //ignite peasant
        {bn::sound_items::cz_dang_z, 
            bn::sound_items::cz_burnimup},
        //stomp
        {bn::sound_items::cz_whee, 
            bn::sound_items::cz_kick_teeth, 
            bn::sound_items::cz_dont_look_good}
    }
};

}