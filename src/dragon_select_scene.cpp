#include "dragon_select_scene.h"
#include "constants.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "sucks.h"
#include "trogdor.h"

#include <bn_keypad.h>
#define SPACING 70
#define MSATIME 10 //move selection animation time 


namespace trog {

dragon_select_scene::dragon_select_scene(session_info &sesh, common_stuff &common_stuff) : 
        _index(0),
        _selection_timer(0),
        _sesh(sesh),
        _common_stuff(common_stuff) {
    _selectable_dragons.emplace_back(dragon::TROGDOR, "TROGDOR", bn::unique_ptr<player>(new trogdor(0,0,_sesh, false, _common_stuff, 0)));
    _selectable_dragons.emplace_back(dragon::SUCKS, "S IS FOR SUCKS", bn::unique_ptr<player>(new sucks(SPACING,0,_sesh, false, _common_stuff, 0)));
    update_text();

    for(auto &opt : _selectable_dragons){
        opt.player_entity->update_anim_action_when_not_moving(true);
        opt.player_entity->set_grayscale(1);
    }

    _selectable_dragons.at(_index).player_entity->set_grayscale(0);
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

    if(_selection_timer == 0){
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
                _selectable_dragons.at(z).player_entity->move_to(MSATIME, (z - _index) * SPACING, 0);
            }    
            _selectable_dragons.at(_index).player_entity->grayscale_to(MSATIME, 0);
            
            int8_t previous_selection_offset = bn::keypad::left_pressed() ? 1 : -1;

            _selectable_dragons.at(_index + previous_selection_offset).player_entity->grayscale_to(MSATIME, 1);

        }

        if(bn::keypad::a_pressed()){
            dragon dtype = _selectable_dragons.at(_index).dragon_type;
            _sesh.set_dragon(dtype);
            _selectable_dragons.at(_index).player_entity.get()->update_anim_action_when_not_moving(false);
            switch(dtype){
                case dragon::TROGDOR:
                    ((trogdor *) _selectable_dragons.at(_index).player_entity.get())->flex();
                    //play trogdor jingle
                break;
                case dragon::SUCKS:
                    //play sucks jingle
                break;
                default:
                break;
            }
            _selection_timer = 1;
        }
    }else if(_selection_timer == 240){
        result = scene_type::PLAY;
    }

    if(_selection_timer) ++_selection_timer;

    return result;
}

}