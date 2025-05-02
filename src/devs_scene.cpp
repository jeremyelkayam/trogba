#include <bn_keypad.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>

#include "bn_sprite_items_butano_big_sprite.h"

#include "devs_scene.h"

namespace trog {

devs_scene::devs_scene() :
        _frame_counter(0) {
    

    _butano.emplace_back(bn::sprite_items::butano_big_sprite.create_sprite(0, -32));
    _butano.emplace_back(bn::sprite_items::butano_big_sprite.create_sprite(0, 32, 1));

    bn::bg_palettes::set_brightness(1);
}

bn::optional<scene_type> devs_scene::update(){
    bn::optional<scene_type> result;
    _frame_counter++;

    short start_time = 300;
    if(start_time < _frame_counter && _frame_counter < start_time + 30){
        bn::bg_palettes::set_brightness(1.0 - (_frame_counter - start_time) * (1.0 / 30));
        
    }


    if(_frame_counter > 360 || bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        bn::bg_palettes::set_brightness(0);
        result = scene_type::LOGO;
    }
    
    return result;
}

}