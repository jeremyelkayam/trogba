#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "gameover_scene.h"

#include "bn_regular_bg_items_trogdead.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"



namespace trog {

gameover_scene::gameover_scene(session_info &sesh, bn::sprite_text_generator &text_generator, bn::sprite_text_generator &big_text_generator) : 
        _dead_trogdor(bn::regular_bg_items::trogdead.create_bg(8, 43)),
        _text_generator(text_generator),
        _big_text_generator(big_text_generator),
        _sesh(sesh) {

    bn::sound_items::gameover.play(TROG_DEFAULT_VOLUME);

    _big_text_generator.set_center_alignment();
    _big_text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
    _big_text_generator.generate(0, -60, "IT'S OVER!", _itsover_text_sprites);

    _text_generator.set_center_alignment();
    // _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
    _text_generator.generate(0, -40, "CHALLENGE AGAIN!", _challengeagain_text_sprites);    
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
}

bn::optional<scene_type> gameover_scene::update(){
    
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        BN_LOG("help?");
        _sesh.mans = TROG_STARTING_LIVES;
        _sesh.score = 0;
        _sesh.level = TROG_STARTING_LEVEL;
        result = scene_type::PLAY;
    }
    // move_background(_dead_trogdor);

    return result;
}

}