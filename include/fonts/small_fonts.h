#pragma once

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_small_font_white.h"
#include "bn_sprite_items_small_font_yellow.h"
#include "bn_sprite_items_small_font_red.h"
#include "bn_sprite_items_small_font_brown.h"
#include "bn_sprite_items_small_font_lightgray.h"
#include "bn_sprite_items_small_font_darkgray.h"
#include "bn_sprite_items_small_font_black.h"

namespace trog
{

constexpr bn::utf8_character small_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};


constexpr int8_t small_font_character_widths[] = {
    6,  // 32
    2,  // 33 !
    4,  // 34 "
    6,  // 35 #
    6,  // 36 $
    5,  // 37 %
    8,  // 38 &
    2,  // 39 '
    3,  // 40 (
    3,  // 41 )
    6,  // 42 *
    6,  // 43 +
    3,  // 44 ,
    6,  // 45 -
    2,  // 46 .
    4,  // 47 /
    6,  // 48 0
    4,  // 49 1
    6,  // 50 2
    6,  // 51 3
    6,  // 52 4
    6,  // 53 5
    6,  // 54 6
    5,  // 55 7
    6,  // 56 8
    6,  // 57 9
    2,  // 58 :
    3,  // 59 ;
    5,  // 60 <
    6,  // 61 =
    5,  // 62 >
    5,  // 63 ?
    6,  // 64 @
    6,  // 65 A
    6,  // 66 B
    6,  // 67 C
    6,  // 68 D
    6,  // 69 E
    6,  // 70 F
    6,  // 71 G
    6,  // 72 H
    3,  // 73 I
    5,  // 74 J
    7,  // 75 K
    5,  // 76 L
    8,  // 77 M
    7,  // 78 N
    7,  // 79 O
    6,  // 80 P
    7,  // 81 Q
    6,  // 82 R
    5,  // 83 S
    7,  // 84 T
    6,  // 85 U
    7,  // 86 V
    8,  // 87 W
    7,  // 88 X
    7,  // 89 Y
    6,  // 90 Z
    4,  // 91 [
    4,  // 92 backslash
    4,  // 93 ]
    7,  // 94 ^
    6,  // 95 _
    4,  // 96 `
    6,  // 97 a
    6,  // 98 b
    5,  // 99 c
    6,  // 100 d
    6,  // 101 e
    4,  // 102 f
    6,  // 103 g
    6,  // 104 h
    3,  // 105 i
    4,  // 106 j
    6,  // 107 k
    3,  // 108 l
    8,  // 109 m
    6,  // 110 n
    6,  // 111 o
    6,  // 112 p
    6,  // 113 q
    5,  // 114 r
    5,  // 115 s
    4,  // 116 t
    6,  // 117 u
    6,  // 118 v
    8,  // 119 w
    6,  // 120 x
    6,  // 121 y
    6,  // 122 z
    6,  // 123 {
    2,  // 124 |
    6,  // 125 }
    8,  // 126 ~
    7,  // Á
    6,  // É
    7,  // Í
    7,  // Ó
    7,  // Ú
    7,  // Ü
    7,  // Ñ
    5,  // á
    5,  // é
    5,  // í
    5,  // ó
    6,  // ú
    6,  // ü
    5,  // ñ
    3,  // ¡
    7,  // ¿
};

constexpr bn::span<const bn::utf8_character> small_font_utf8_characters_span(
        small_font_utf8_characters);

constexpr auto small_font_utf8_characters_map =
        bn::utf8_characters_map<small_font_utf8_characters_span>();

constexpr bn::sprite_font small_font_white(
        bn::sprite_items::small_font_white, small_font_utf8_characters_map.reference(),
        small_font_character_widths);

constexpr bn::sprite_font small_font_yellow(
        bn::sprite_items::small_font_yellow, small_font_utf8_characters_map.reference(),
        small_font_character_widths);

constexpr bn::sprite_font small_font_red(
        bn::sprite_items::small_font_red, small_font_utf8_characters_map.reference(),
        small_font_character_widths);
        
constexpr bn::sprite_font small_font_brown(
    bn::sprite_items::small_font_brown, small_font_utf8_characters_map.reference(),
    small_font_character_widths);
    
constexpr bn::sprite_font small_font_lightgray(
    bn::sprite_items::small_font_lightgray, small_font_utf8_characters_map.reference(),
    small_font_character_widths);
    
constexpr bn::sprite_font small_font_darkgray(
    bn::sprite_items::small_font_darkgray, small_font_utf8_characters_map.reference(),
    small_font_character_widths);
    
constexpr bn::sprite_font small_font_black(
    bn::sprite_items::small_font_black, small_font_utf8_characters_map.reference(),
    small_font_character_widths);

}
