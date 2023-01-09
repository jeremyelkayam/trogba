/*
 * Based on common_fixed_8x16_sprite_font.h 
 * (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#pragma once

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_trogdor_fixed_8x16_font.h"

namespace trog
{

constexpr bn::utf8_character fixed_8x16_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr bn::span<const bn::utf8_character> fixed_8x16_sprite_font_utf8_characters_span(
        fixed_8x16_sprite_font_utf8_characters);

constexpr auto fixed_8x16_sprite_font_utf8_characters_map =
        bn::utf8_characters_map<fixed_8x16_sprite_font_utf8_characters_span>();

constexpr bn::sprite_font fixed_8x16_sprite_font(
        bn::sprite_items::trogdor_fixed_8x16_font, fixed_8x16_sprite_font_utf8_characters_map.reference());

}

