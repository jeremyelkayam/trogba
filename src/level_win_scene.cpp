#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "level_win_scene.h"

#include "bn_regular_bg_items_trogsmile.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"



namespace trog {

level_win_scene::level_win_scene(session_info &sesh, bn::sprite_text_generator &text_generator) : 
        _happy_trogdor(bn::regular_bg_items::trogsmile.create_bg(8, 43)),
        _text_generator(text_generator),
        _sesh(sesh) {

    bn::sound_items::burninate.play(TROG_DEFAULT_VOLUME);

    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    _text_generator.generate(60, -10, "nice work!", _nicework_text_sprites);
    _text_generator.generate(60, 10, "LEVEL BEATED!", _levelbeated_text_sprites);
}

bn::optional<scene_type> level_win_scene::update(){
    
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        BN_LOG("help?");
        _sesh.advance_level();
        result = scene_type::PLAY;
    }
    // move_background(_happy_trogdor);

    return result;
}

}