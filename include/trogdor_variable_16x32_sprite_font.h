/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#pragma once

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_trogdor_variable_16x32_font.h"

namespace trog
{

constexpr bn::utf8_character variable_16x32_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr int8_t variable_16x32_sprite_font_character_widths[] = {
    12,  // 32
    4,  // 33 !
    14,  // 34 "
    16,  // 35 #
    16,  // 36 $
    14,  // 37 %
    16,  // 38 &
    4,  // 39 '
    10,  // 40 (
    10,  // 41 )
    14,  // 42 *
    16,  // 43 +
    4,  // 44 ,
    14,  // 45 -
    4,  // 46 .
    10,  // 47 /
    16,  // 48 0
    16,  // 49 1
    16,  // 50 2
    16,  // 51 3
    16,  // 52 4
    16,  // 53 5
    16,  // 54 6
    16,  // 55 7
    16,  // 56 8
    16,  // 57 9
    2,  // 58 :
    8,  // 59 ;
    12,  // 60 <
    14,  // 61 =
    12,  // 62 >
    14,  // 63 ?
    16,  // 64 @
    16,  // 65 A
    16,  // 66 B
    16,  // 67 C
    16,  // 68 D
    16,  // 69 E
    16,  // 70 F
    16,  // 71 G
    16,  // 72 H
    16,  // 73 I
    16,  // 74 J
    16,  // 75 K
    16,  // 76 L
    16,  // 77 M
    16,  // 78 N
    16,  // 79 O
    16,  // 80 P
    16,  // 81 Q
    16,  // 82 R
    16,  // 83 S
    16,  // 84 T
    16,  // 85 U
    16,  // 86 V
    16,  // 87 W
    16,  // 88 X
    16,  // 89 Y
    16,  // 90 Z
    10,  // 91 [
    14,  // 92
    10,  // 93 ]
    8,  // 94 ^
    14,  // 95 _
    8,  // 96 `
    16,  // 97 a
    16,  // 98 b
    16,  // 99 c
    16,  // 100 d
    16,  // 101 e
    16,  // 102 f
    16,  // 103 g
    16,  // 104 h
    16,  // 105 i
    16,  // 106 j
    16,  // 107 k
    16,  // 108 l
    16,  // 109 m
    16,  // 110 n
    16,  // 111 o
    16,  // 112 p
    16,  // 113 q
    16,  // 114 r
    16,  // 115 s
    16,  // 116 t
    16,  // 117 u
    16,  // 118 v
    16,  // 119 w
    16,  // 120 x
    16,  // 121 y
    16,  // 122 z
    14,  // 123 {
    6,  // 124 |
    12,  // 125 }
    16,  // 126 ~
    14,  // Á
    14,  // É
    10,  // Í
    14,  // Ó
    14,  // Ú
    14,  // Ü
    14,  // Ñ
    14,  // á
    14,  // é
    8,  // í
    14,  // ó
    14,  // ú
    14,  // ü
    14,  // ñ
    6,  // ¡
    14,  // ¿
};

constexpr bn::span<const bn::utf8_character> variable_16x32_sprite_font_utf8_characters_span(
        variable_16x32_sprite_font_utf8_characters);

constexpr auto variable_16x32_sprite_font_utf8_characters_map =
        bn::utf8_characters_map<variable_16x32_sprite_font_utf8_characters_span>();

constexpr bn::sprite_font variable_16x32_sprite_font(
        bn::sprite_items::trogdor_variable_16x32_font, variable_16x32_sprite_font_utf8_characters_map.reference(),
        variable_16x32_sprite_font_character_widths);

}
