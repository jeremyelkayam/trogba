#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_blending.h>
#include <bn_sprite_palettes.h>

#include "gameover_scene.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_regular_bg_items_trogdead.h"


namespace trog {

gameover_scene::gameover_scene(session_info &sesh, bn::sprite_text_generator &generator) : 
        _sesh(sesh),
        _itsover_text(false, 0, -50, "IT'S OVER!",
            bn::sprite_items::trogdor_variable_8x16_font_red.palette_item(),
            bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item(), generator),
        _dead_trogdor(bn::regular_bg_items::trogdead.create_bg(8, 43)) {
}


bn::optional<scene_type> gameover_scene::update(){
    _itsover_text.update();
    
    bn::optional<scene_type> result;

    if(bn::keypad::a_pressed()){
        _sesh.reset();
        result = scene_type::INSTRUCTIONS;
    }

    return result;
}

}