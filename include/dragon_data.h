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

#define TDATA dragons[(int)dragon::TROGDOR]
#define SDATA dragons[(int)dragon::SUCKS]
#define CDATA dragons[(int)dragon::CHIAROSCURO]
#define WDATA dragons[(int)dragon::WORMDINGLER]

namespace trog
{

struct dragon_data
{
    const char *name, *ability, *join_phrase;
    const bn::music_item jingle;
    const bn::regular_bg_item smile, dead;
    const bn::fixed width, height, speed;
};


constexpr const dragon_data dragons[4] = 
{
    {
        "TROGDOR",
        "none",
        "Strikes Again!",
        bn::music_items::trog,
        bn::regular_bg_items::trogsmile,
        bn::regular_bg_items::trogdead,
        24, 34, TROG_TROGDOR_SPEED //0.87
    },
    {
        "S IS FOR SUCKS",
        "ground pound",
        "",
        bn::music_items::sucks,
        bn::regular_bg_items::sucksmile,
        bn::regular_bg_items::suckdead,
        28, 40, 0.7
    },
    {
        "CHIAROSCURO",
        "speedy",
        "Improves on your Methods!",
        bn::music_items::trog,
        bn::regular_bg_items::chiarosmile,
        bn::regular_bg_items::chiarodead,
        24, 34, 0.87
    },
    {
        "WORMDINGLER",
        "extendo tongue",
        "Uh... Not gonna touch this one",
        bn::music_items::wormdingler,
        bn::regular_bg_items::wormsmile,
        bn::regular_bg_items::wormdead,
        42, 16, 0.7
    }
};

}