/*
 * enums.h
 * Patterned after fr_scene_type.h (2020-2022 Gustavo Valiente, gustavo.valiente@protonmail.com). Used under zlib License.
 */

#pragma once

#include "bn_common.h"

namespace trog {
    enum class scene_type
    {
        DEVS,
        LOGO,
        TITLE,
        PLAY,
        BONUS,
        LEVELBEAT,
        LOSE,
        FIREYRAGE,
        MOVIE,
        MENU,
        HISCORES,
        CREDITS,
        OPTIONS,
        DRAGON_SELECT,
        CUTSCENE_VIEWER,
        EXTRAS
    };
    enum class direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    enum class dragon 
    {
        TROGDOR,
        SUCKS,
        CHIAROSCURO,
        WORMDINGLER
    };
}
