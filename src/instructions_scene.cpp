#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include "common_variable_8x8_sprite_font.h"
#include "instructions_scene.h"
#include "scene_type.h"


namespace trog {

instructions_scene::instructions_scene(){

    bn::sound_items::trogador.play(1);
    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    text_generator.set_center_alignment();

    bn::vector<bn::sprite_ptr, 32> text_sprites;
    text_generator.generate(0, 100, "press a to continute", text_sprites);
}

bn::optional<scene_type> instructions_scene::update(){
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        result = scene_type::PLAY;
    }
    return result;
}

}