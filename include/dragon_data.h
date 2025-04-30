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

namespace trog
{

struct dragon_data
{
    const char *name, *ability, *join_phrase;
    const bn::music_item jingle;
    const bn::regular_bg_item smile, dead;
    const bn::fixed height;
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
        34
    },
    {
        "S IS FOR SUCKS",
        "ground pound",
        "",
        bn::music_items::sucks,
        bn::regular_bg_items::sucksmile,
        bn::regular_bg_items::suckdead,
        40
    },
    {
        "CHIAROSCURO",
        "speedy",
        "Improves on your Methods",
        bn::music_items::trog,
        bn::regular_bg_items::chiarosmile,
        bn::regular_bg_items::chiarodead,
        34,
    },
    {
        "WORMDINGLER",
        "extendo tongue",
        "Gives A Care!",
        bn::music_items::wormdingler,
        bn::regular_bg_items::wormsmile,
        bn::regular_bg_items::wormdead,
        16
    }
};

}