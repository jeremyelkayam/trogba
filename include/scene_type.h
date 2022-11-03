/*
 * scene_type.h
 * Patterned after fr_scene_type.h (2020-2022 Gustavo Valiente, gustavo.valiente@protonmail.com). Used under zlib License.
 */

#pragma once

#include "bn_common.h"

namespace trog {
    enum class scene_type
    {
        TITLE,
        INSTRUCTIONS,
        PLAY,
        LEVELBEAT,
        LOSE,
        MOVIE
    };
}
