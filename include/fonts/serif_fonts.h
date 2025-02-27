#pragma once

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_serif_font_white.h"
#include "bn_sprite_items_serif_font_yellow.h"
#include "bn_sprite_items_serif_font_red.h"
#include "bn_sprite_items_serif_font_brown.h"
#include "bn_sprite_items_serif_font_lightgray.h"
#include "bn_sprite_items_serif_font_darkgray.h"
#include "bn_sprite_items_serif_font_black.h"

namespace trog
{

constexpr bn::utf8_character serif_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};


constexpr int8_t serif_font_character_widths[] = {
    6,  // 32
    7,  // 33 !
    7,  // 34 "
    8,  // 35 #
    8,  // 36 $
    7,  // 37 %
    8,  // 38 &
    2,  // 39 '
    5,  // 40 (
    5,  // 41 )
    7,  // 42 *
    8,  // 43 +
    2,  // 44 ,
    8,  // 45 -
    2,  // 46 .
    5,  // 47 /
    8,  // 48 0
    8,  // 49 1
    8,  // 50 2
    8,  // 51 3
    8,  // 52 4
    8,  // 53 5
    8,  // 54 6
    8,  // 55 7
    8,  // 56 8
    8,  // 57 9
    2,  // 58 :
    4,  // 59 ;
    6,  // 60 <
    7,  // 61 =
    6,  // 62 >
    7,  // 63 ?
    8,  // 64 @
    8,  // 65 A
    8,  // 66 B
    8,  // 67 C
    8,  // 68 D
    8,  // 69 E
    8,  // 70 F
    8,  // 71 G
    8,  // 72 H
    8,  // 73 I
    8,  // 74 J
    8,  // 75 K
    8,  // 76 L
    8,  // 77 M
    8,  // 78 N
    8,  // 79 O
    8,  // 80 P
    8,  // 81 Q
    8,  // 82 R
    8,  // 83 S
    8,  // 84 T
    8,  // 85 U
    8,  // 86 V
    8,  // 87 W
    8,  // 88 X
    8,  // 89 Y
    8,  // 90 Z
    4,  // 91 [
    7,  // 92
    4,  // 93 ]
    4,  // 94 ^
    7,  // 95 _
    4,  // 96 `
    8,  // 97 a
    8,  // 98 b
    8,  // 99 c
    8,  // 100 d
    8,  // 101 e
    8,  // 102 f
    8,  // 103 g
    8,  // 104 h
    8,  // 105 i
    8,  // 106 j
    8,  // 107 k
    8,  // 108 l
    8,  // 109 m
    8,  // 110 n
    8,  // 111 o
    8,  // 112 p
    8,  // 113 q
    8,  // 114 r
    8,  // 115 s
    8,  // 116 t
    8,  // 117 u
    8,  // 118 v
    8,  // 119 w
    8,  // 120 x
    8,  // 121 y
    8,  // 122 z
    6,  // 123 {
    3,  // 124 |
    6,  // 125 }
    8,  // 126 ~
    7,  // Á
    7,  // É
    5,  // Í
    7,  // Ó
    7,  // Ú
    7,  // Ü
    7,  // Ñ
    7,  // á
    7,  // é
    4,  // í
    7,  // ó
    7,  // ú
    7,  // ü
    7,  // ñ
    3,  // ¡
    7,  // ¿
};

constexpr bn::span<const bn::utf8_character> serif_font_utf8_characters_span(
        serif_font_utf8_characters);

constexpr auto serif_font_utf8_characters_map =
        bn::utf8_characters_map<serif_font_utf8_characters_span>();

constexpr bn::sprite_font serif_font_white(
        bn::sprite_items::serif_font_white, serif_font_utf8_characters_map.reference(),
        serif_font_character_widths);

constexpr bn::sprite_font serif_font_yellow(
        bn::sprite_items::serif_font_yellow, serif_font_utf8_characters_map.reference(),
        serif_font_character_widths);

constexpr bn::sprite_font serif_font_red(
        bn::sprite_items::serif_font_red, serif_font_utf8_characters_map.reference(),
        serif_font_character_widths);
        
constexpr bn::sprite_font serif_font_brown(
    bn::sprite_items::serif_font_brown, serif_font_utf8_characters_map.reference(),
    serif_font_character_widths);
    
constexpr bn::sprite_font serif_font_lightgray(
    bn::sprite_items::serif_font_lightgray, serif_font_utf8_characters_map.reference(),
    serif_font_character_widths);
    
constexpr bn::sprite_font serif_font_darkgray(
    bn::sprite_items::serif_font_darkgray, serif_font_utf8_characters_map.reference(),
    serif_font_character_widths);
    
constexpr bn::sprite_font serif_font_black(
    bn::sprite_items::serif_font_black, serif_font_utf8_characters_map.reference(),
    serif_font_character_widths);

}
