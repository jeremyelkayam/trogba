#include "dragon_select_scene.h"
#include "constants.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "sucks.h"
#include "trogdor.h"

#include <bn_keypad.h>
#define SPACING 60


namespace trog {

dragon_select_scene::dragon_select_scene(session_info &sesh, common_stuff &common_stuff) : 
        _index(0),
        _sesh(sesh),
        _common_stuff(common_stuff) {
    _selectable_dragons.emplace_back(dragon::TROGDOR, "TROGDOR", bn::unique_ptr<player>(new trogdor(0,0,_sesh, false, _common_stuff, 0)));
    _selectable_dragons.emplace_back(dragon::SUCKS, "S IS FOR SUCKS", bn::unique_ptr<player>(new sucks(SPACING,0,_sesh, false, _common_stuff, 0)));
    update_text();

    for(auto &opt : _selectable_dragons){
        opt.player_entity->update_anim_action_when_not_moving(true);
    }
}

void dragon_select_scene::update_text(){
    _selected_text.clear();
    _common_stuff.text_generator.set_palette_item(WHITE_PALETTE);
    _common_stuff.text_generator.generate(0, -60, _selectable_dragons.at(_index).name, _selected_text);
}

bn::optional<scene_type> dragon_select_scene::update(){

    for(auto &opt : _selectable_dragons){
        opt.player_entity->update_anim();
    }

    bn::optional<scene_type> result;

    if(bn::keypad::left_pressed()){
        if(_index != 0) _index--;

        //looping code
        // if(_index == 0)
        //     _index = _selectable_dragons.size() - 1;
        // else _index--;
    }if(bn::keypad::right_pressed()){
        if(_index != _selectable_dragons.size() - 1) _index++;
        //looping code
        // if(_index == _selectable_dragons.size() - 1)
        //     _index = 0;
        // else _index++;
    }

    if(bn::keypad::left_pressed() || bn::keypad::right_pressed()) {
        update_text();
        for(uint8_t z = 0; z < _selectable_dragons.size(); z++){
            _selectable_dragons.at(z).player_entity->move_to(10, (z - _index) * SPACING, 0);
        }    
    }

    if(bn::keypad::a_pressed()){
        _sesh.set_dragon(_selectable_dragons.at(_index).dragon_type);
        result = scene_type::PLAY;
    }

    return result;
}

}