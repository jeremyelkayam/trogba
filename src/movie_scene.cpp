#include <bn_keypad.h>
#include "movie_scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"

namespace trog{

movie_scene::movie_scene(session_info &sesh, bn::sprite_text_generator &text_generator) : 
    _sesh(sesh) {
    
    text_generator.set_center_alignment();
    text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());    text_generator.set_center_alignment();
    
    text_generator.generate(0, -50, "congrat", _text_sprites);

    text_generator.generate(0, -25, "u won game", _text_sprites);

    text_generator.generate(0, 0, "good job", _text_sprites);

    text_generator.generate(0, 25, "do it again", _text_sprites);

}

bn::optional<scene_type> movie_scene::update(){
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed() && _sesh.get_level() == 101){
        _sesh.reset();
        result = scene_type::TITLE;
    }
    // move_background(_dead_trogdor);

    return result;
}

}