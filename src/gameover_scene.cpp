#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "gameover_scene.h"


namespace trog {

gameover_scene::gameover_scene() : 
        _dead_trogdor(bn::regular_bg_items::trogdead.create_bg(0, 0)) {

    bn::sound_items::gameover.play(TROG_DEFAULT_VOLUME);
}

bn::optional<scene_type> gameover_scene::update(){
    
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        result = scene_type::PLAY;
    }
    return result;
}

}