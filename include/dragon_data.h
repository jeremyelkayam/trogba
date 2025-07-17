#pragma once

#include "bn_music_items.h"
#include "bn_regular_bg_items_trogsmile.h"
#include "bn_regular_bg_items_trogdead.h"
#include "bn_regular_bg_items_sucksmile.h"
#include "bn_regular_bg_items_suckdead.h"
#include "bn_regular_bg_items_chiarosmile.h"
#include "bn_regular_bg_items_chiarodead.h"
#include "bn_regular_bg_items_wormsmile.h"
#include "bn_regular_bg_items_wormdead.h"
#include "bn_sprite_items_trogdorhead.h"
#include "bn_sprite_items_suckshead.h"
#include "bn_sprite_items_chiarohead.h"
#include "bn_sprite_items_wormhead.h"
#include "bn_sprite_items_strongbad.h"
#include "bn_sprite_items_strongsad.h"
#include "bn_sprite_items_coach_z.h"

#define TDATA dragons[(int)dragon::TROGDOR]
#define SDATA dragons[(int)dragon::SUCKS]
#define CDATA dragons[(int)dragon::CHIAROSCURO]
#define WDATA dragons[(int)dragon::WORMDINGLER]

namespace trog
{

struct dragon_data
{
    const char *name, *ability, *join_phrase;
    const char *end_phrase[3];
    const bn::music_item jingle;
    const bn::sound_item congrats;
    const bn::sprite_item head_icon, artist_sprite;
    const bn::regular_bg_item smile, dead;
    const bn::fixed width, height, speed;
};


constexpr const dragon_data dragons[4] = 
{
    {
        "TROGDOR",
        "none",
        "Strikes Again!",
        {"congratulations.", "you got", "good score"},
        bn::music_items::trog,
        bn::sound_items::cutscene_congrats,
        bn::sprite_items::trogdorhead,
        bn::sprite_items::strongbad,
        bn::regular_bg_items::trogsmile,
        bn::regular_bg_items::trogdead,
        24, 34, TROG_TROGDOR_SPEED //0.87
    },
    {
        "S IS FOR SUCKS",
        "ground pound",
        "",
        {"congratulations.", "you got", "good score"},
        bn::music_items::sucks,
        bn::sound_items::cutscene_congrats,
        bn::sprite_items::suckshead,
        bn::sprite_items::strongbad,
        bn::regular_bg_items::sucksmile,
        bn::regular_bg_items::suckdead,
        28, 40, 0.7
    },
    {
        "CHIAROSCURO",
        "speedy",
        "Improves on your Methods!",
        {"i guess you won.", "you beat", "that score"},
        bn::music_items::trog,
        bn::sound_items::ss_congrats,
        bn::sprite_items::chiarohead,
        bn::sprite_items::strongsad,
        bn::regular_bg_items::chiarosmile,
        bn::regular_bg_items::chiarodead,
        36, 40, 1
    },
    {
        "WORMDINGLER",
        "extendo tongue",
        "Uh... Not gonna touch this one",
        {"congratulations.", "you did", "great jorb"},
        bn::music_items::wormdingler,
        bn::sound_items::cz_congrats,
        bn::sprite_items::wormhead,
        bn::sprite_items::coach_z,
        bn::regular_bg_items::wormsmile,
        bn::regular_bg_items::wormdead,
        42, 16, 0.7
    }
};

}