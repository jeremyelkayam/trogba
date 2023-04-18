/*
 * Based on common_fixed_8x8_sprite_font.h 
 * by Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#pragma once

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_common_fixed_8x8_font.h"

namespace trog
{

constexpr bn::sprite_font fixed_8x8_sprite_font(
        bn::sprite_items::common_fixed_8x8_font);

}
