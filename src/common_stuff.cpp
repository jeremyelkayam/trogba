#include "common_stuff.h"
#include "trogdor_variable_8x16_sprite_font.h"
#include "trogdor_variable_32x64_sprite_font.h"
#include "trogdor_variable_8x8_sprite_font.h"
namespace trog { 


common_stuff::common_stuff() : 
    text_generator(trog::variable_8x16_sprite_font),
    big_generator(trog::variable_32x64_sprite_font),
    small_generator(trog::variable_8x8_sprite_font) { 

}

}
